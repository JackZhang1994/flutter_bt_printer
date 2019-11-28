package com.jvtd.flutter_bt_printer.utils;

import android.app.AlertDialog;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Handler;
import android.text.TextUtils;
import android.widget.Toast;

import com.dothantech.lpapi.IAtBitmap;
import com.dothantech.lpapi.LPAPI;
import com.dothantech.printer.IDzPrinter;

/**
 * Description:
 * Author: Jack Zhang
 * create on: 2019-11-28 13:53
 */
public class LPUtil
{
  private static final String KeyLastPrinterMac = "LastPrinterMac";
  private static final String KeyLastPrinterName = "LastPrinterName";
  private static final String KeyLastPrinterType = "LastPrinterType";

  private static volatile LPUtil instance = null;
  private final Context mContext;
  private final Handler mHandler;
  // 上次连接成功的设备对象
  private IDzPrinter.PrinterAddress mPrinterAddress = null;
  // 状态提示框
  private AlertDialog stateAlertDialog = null;
  private LPAPI api;


  private LPUtil(Context context)
  {
    this.mContext = context;
    // 用于处理各种通知消息，刷新界面的handler
    this.mHandler = new Handler();
  }

  //运行时加载对象
  public static LPUtil getInstance(Context context)
  {
    if (instance == null)
    {
      synchronized (LPUtil.class)
      {
        if (instance == null)
        {
          instance = new LPUtil(context);
        }
      }
    }
    return instance;
  }

  /********************************************************************************************************************************************/
  // DzPrinter连接打印功能相关
  /********************************************************************************************************************************************/

  // LPAPI 打印机操作相关的回调函数。

  private final LPAPI.Callback mCallback = new LPAPI.Callback()
  {

    /***************************************************************************************************************************************/
    // 所有回调函数都是在打印线程中被调用，因此如果需要刷新界面，需要发送消息给界面主线程，以避免互斥等繁琐操作。

    /****************************************************************************************************************************************/

    // 打印机连接状态发生变化时被调用
    @Override
    public void onStateChange(IDzPrinter.PrinterAddress arg0, IDzPrinter.PrinterState arg1)
    {
      final IDzPrinter.PrinterAddress printer = arg0;
      switch (arg1)
      {
        case Connected:
        case Connected2:
          // 打印机连接成功，发送通知，刷新界面提示
          mHandler.post(() -> onPrinterConnected(printer));
          break;

        case Disconnected:
          // 打印机连接失败、断开连接，发送通知，刷新界面提示
          mHandler.post(() -> onPrinterDisconnected());
          break;

        default:
          break;
      }
    }

    // 蓝牙适配器状态发生变化时被调用
    @Override
    public void onProgressInfo(IDzPrinter.ProgressInfo arg0, Object arg1)
    {
    }

    @Override
    public void onPrinterDiscovery(IDzPrinter.PrinterAddress arg0, IDzPrinter.PrinterInfo arg1)
    {
    }

    // 打印标签的进度发生变化是被调用
    @Override
    public void onPrintProgress(IDzPrinter.PrinterAddress address, Object bitmapData, IDzPrinter.PrintProgress progress, Object addiInfo)
    {
      switch (progress)
      {
        case Success:
          // 打印标签成功，发送通知，刷新界面提示
          mHandler.post(() -> onPrintSuccess());
          break;

        case Failed:
          // 打印标签失败，发送通知，刷新界面提示
          mHandler.post(() -> onPrintFailed());
          break;

        default:
          break;
      }
    }
  };

  public void init()
  {
    // 调用LPAPI对象的init方法初始化对象
    this.api = LPAPI.Factory.createInstance(mCallback);

    SharedPreferences sharedPreferences = mContext.getSharedPreferences("BOP", Context.MODE_PRIVATE);
    String lastPrinterMac = sharedPreferences.getString(KeyLastPrinterMac, null);
    String lastPrinterName = sharedPreferences.getString(KeyLastPrinterName, null);
    String lastPrinterType = sharedPreferences.getString(KeyLastPrinterType, null);
    IDzPrinter.AddressType lastAddressType = TextUtils.isEmpty(lastPrinterType) ? null : Enum.valueOf(IDzPrinter.AddressType.class, lastPrinterType);
    if (lastPrinterMac == null || lastPrinterName == null || lastAddressType == null)
    {
//      mPrinterAddress = null;
      mPrinterAddress = api.getFirstPrinter();
    } else
    {
      mPrinterAddress = new IDzPrinter.PrinterAddress(lastPrinterName, lastPrinterMac, lastAddressType);
    }

    // 尝试连接上次成功连接的打印机
    if (mPrinterAddress != null)
    {
      if (api.openPrinterByAddress(mPrinterAddress))
      {
        // 连接打印机的请求提交成功，刷新界面提示
        onPrinterConnecting(mPrinterAddress, false);
        return;
      }
    }
  }

  public void print(String topTitle, String bottomTitle, String qrCode, String label1Title, String label1Value, String label2Title, String label2Value, String label3Title, String label3Value)
  {
    if (mPrinterAddress == null)
    {
      IDzPrinter.PrinterAddress printerAddress = api.getFirstPrinter();
      if (printerAddress == null)
      {
        Toast.makeText(mContext, "请先与蓝牙打印机进行配对", Toast.LENGTH_SHORT).show();
        return;
      } else
      {
        mPrinterAddress = printerAddress;
        if (api.openPrinterByAddress(mPrinterAddress))
        {
          // 连接打印机的请求提交成功，刷新界面提示
          onPrinterConnecting(mPrinterAddress, false);
          return;
        }
      }
    }

    IDzPrinter.PrinterState state = api.getPrinterState();
    if (state == null)
    {
      Toast.makeText(mContext, "蓝牙打印机连接异常，请重新连接", Toast.LENGTH_SHORT).show();
      return;
    } else
    {
      if (state == IDzPrinter.PrinterState.Connecting)
      {
        Toast.makeText(mContext, "设备连接中，请稍后再试", Toast.LENGTH_SHORT).show();
        return;
      } else if (state == IDzPrinter.PrinterState.Printing)
      {
        Toast.makeText(mContext, "当前设备正在打印，请稍后再试", Toast.LENGTH_SHORT).show();
        return;
      } else if (state == IDzPrinter.PrinterState.Working)
      {
        Toast.makeText(mContext, "当前设备状态暂不支持打印，请检查后重试", Toast.LENGTH_SHORT).show();
        return;
      } else if (state == IDzPrinter.PrinterState.Disconnected)
      {
        if (api.openPrinterByAddress(mPrinterAddress))
        {
          // 连接打印机的请求提交成功，刷新界面提示
          onPrinterConnecting(mPrinterAddress, false);
          return;
        } else
        {
          Toast.makeText(mContext, "当前设备状态暂不支持打印，请检查后重试", Toast.LENGTH_SHORT).show();
          return;
        }
      } else
      {
        // 打印机已连接
        if (printCustom(topTitle, bottomTitle, qrCode, label1Title, label1Value, label2Title, label2Value, label3Title, label3Value))
          onPrintStart();
        else
          onPrintFailed();
      }
    }
  }

  public void quit()
  {
    // 应用退出时，调用LPAPI对象的quit方法断开打印机连接
    api.quit();

    // 应用退出时需要的操作
    // 保存相关信息
    SharedPreferences sharedPreferences = mContext.getSharedPreferences("BOP", Context.MODE_PRIVATE);
    SharedPreferences.Editor editor = sharedPreferences.edit();

    if (mPrinterAddress != null)
    {
      editor.putString(KeyLastPrinterMac, mPrinterAddress.macAddress);
      editor.putString(KeyLastPrinterName, mPrinterAddress.shownName);
      editor.putString(KeyLastPrinterType, mPrinterAddress.addressType.toString());
    }
    editor.commit();
  }

  /**
   * 打印自定义
   */
  private boolean printCustom(String topTitle, String bottomTitle, String qrCode, String label1Title, String label1Value, String label2Title, String label2Value, String label3Title, String label3Value)
  {
    double lHorLen = 57;// 线横向长度
    double lVerLen = 38;// 线纵向长度
    double lWidth = 0.2;// 线宽度
    double cVerLen = 7.6;// 单元格纵向长度
    double cHorLen = 18;// 单元格横向宽度
    double qrCodeHorLen = cVerLen * 3;

    // 开始绘图任务，传入参数(页面宽度, 页面高度)
    api.startJob(lHorLen, lVerLen, 90);
    api.setItemHorizontalAlignment(IAtBitmap.ItemAlignment.CENTER);
    api.setItemVerticalAlignment(IAtBitmap.ItemAlignment.CENTER);

    api.drawRectangle(0, 0, lHorLen, lVerLen, lWidth);

    // 画横线
    api.drawLine(0, cVerLen, lHorLen, cVerLen, lWidth);
    api.drawLine(0, lVerLen - cVerLen, lHorLen, lVerLen - cVerLen, lWidth);
    api.drawLine(qrCodeHorLen, 2 * cVerLen, lHorLen, 2 * cVerLen, lWidth);
    api.drawLine(qrCodeHorLen, 3 * cVerLen, lHorLen, 3 * cVerLen, lWidth);
    // 画竖线
    api.drawLine(qrCodeHorLen, cVerLen, qrCodeHorLen, lVerLen - cVerLen, lWidth);
    api.drawLine(qrCodeHorLen + cHorLen, cVerLen, qrCodeHorLen + cHorLen, lVerLen - cVerLen, lWidth);

    api.drawText(topTitle, 0, 0, lHorLen, cVerLen, 3.2);
    api.draw2DQRCode(qrCode, 2, cVerLen + 2, cHorLen);

    api.drawText(label1Title, qrCodeHorLen, cVerLen, cHorLen, cVerLen, 2.5);
    api.drawText(label1Value, qrCodeHorLen + cHorLen, cVerLen, cHorLen, cVerLen, 2.5);

    api.drawText(label2Title, qrCodeHorLen, cVerLen * 2, cHorLen, cVerLen, 2.5);
    api.drawText(label2Value, qrCodeHorLen + cHorLen, cVerLen * 2, cHorLen, cVerLen, 2.5);

    api.drawText(label3Title, qrCodeHorLen, cVerLen * 3, cHorLen, cVerLen, 2.5);
    api.drawText(label3Value, qrCodeHorLen + cHorLen, cVerLen * 3, cHorLen, cVerLen, 2.5);

    api.drawText(bottomTitle, 0, cVerLen * 4, lHorLen, cVerLen, 2.5);

    // 结束绘图任务提交打印
    return api.commitJob();
  }

  // 连接打印机请求成功提交时操作
  private void onPrinterConnecting(IDzPrinter.PrinterAddress printer, boolean showDialog)
  {
    // 连接打印机请求成功提交，刷新界面提示
    String txt = printer.shownName;
    if (TextUtils.isEmpty(txt))
      txt = printer.macAddress;
    txt = "正在连接" + '[' + txt + ']' + "打印机";
    if (showDialog)
    {
      showStateAlertDialog(txt);
    }
  }

  // 连接打印机成功时操作
  private void onPrinterConnected(IDzPrinter.PrinterAddress printer)
  {
    // 连接打印机成功时，刷新界面提示，保存相关信息
    clearAlertDialog();
    Toast.makeText(mContext, "连接打印机成功", Toast.LENGTH_SHORT).show();
    mPrinterAddress = printer;
  }

  // 连接打印机操作提交失败、打印机连接失败或连接断开时操作
  private void onPrinterDisconnected()
  {
    // 连接打印机操作提交失败、打印机连接失败或连接断开时，刷新界面提示
    clearAlertDialog();

    Toast.makeText(mContext, "连接打印机失败", Toast.LENGTH_SHORT).show();
  }

  // 开始打印标签时操作
  private void onPrintStart()
  {
    // 开始打印标签时，刷新界面提示
    showStateAlertDialog("正在打印标签");
  }

  // 标签打印成功时操作
  private void onPrintSuccess()
  {
    // 标签打印成功时，刷新界面提示
    clearAlertDialog();
    Toast.makeText(mContext, "标签打印成功", Toast.LENGTH_SHORT).show();
  }

  // 打印请求失败或标签打印失败时操作
  private void onPrintFailed()
  {
    // 打印请求失败或标签打印失败时，刷新界面提示
    clearAlertDialog();
    Toast.makeText(mContext, "标签打印失败", Toast.LENGTH_SHORT).show();
  }

  // 显示连接、打印的状态提示框
  private void showStateAlertDialog(String str)
  {
    if (stateAlertDialog != null && stateAlertDialog.isShowing())
    {
      stateAlertDialog.setTitle(str);
    } else
    {
      stateAlertDialog = new AlertDialog.Builder(mContext).setCancelable(false).setTitle(str).show();
    }
  }

  // 清除连接、打印的状态提示框
  private void clearAlertDialog()
  {
    if (stateAlertDialog != null && stateAlertDialog.isShowing())
    {
      stateAlertDialog.dismiss();
    }
    stateAlertDialog = null;
  }
}
