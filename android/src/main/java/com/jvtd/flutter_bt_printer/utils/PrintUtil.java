package com.jvtd.flutter_bt_printer.utils;

import android.app.AlertDialog;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;

import com.dothantech.common.DzEnum;
import com.dothantech.common.DzToast;
import com.dothantech.lpapi.IAtBitmap;
import com.dothantech.printer.IDzPrinter;

/**
 * Description:
 * Author: Jack Zhang
 * create on: 2019-11-25 13:22
 */
public class PrintUtil
{
  private static final String KeyLastPrinterMac = "LastPrinterMac";
  private static final String KeyLastPrinterName = "LastPrinterName";
  private static final String KeyLastPrinterType = "LastPrinterType";
  private final Context mContext;
  private Handler mHandler;

  private IDzPrinter.PrinterAddress mPrinterAddress;

  private static volatile PrintUtil instance = null;
  private AlertDialog stateAlertDialog;

  private PrintUtil(Context context)
  {
    this.mContext = context;
    checkHandler();
  }

  private void checkHandler()
  {
    if (mHandler == null)
    {
      mHandler = new Handler();
    }
  }

  //运行时加载对象
  public static PrintUtil getInstance(Context context)
  {
    if (instance == null)
    {
      synchronized (PrintUtil.class)
      {
        if (instance == null)
        {
          instance = new PrintUtil(context);
        }
      }
    }
    return instance;
  }

  public void init()
  {
    IDzPrinter.Factory.getInstance().init(mContext, new IDzPrinter.IDzPrinterCallback()
    {

      @Override
      public void onProgressInfo(IDzPrinter.ProgressInfo progressInfo, Object o)
      {

      }

      @Override
      public void onStateChange(IDzPrinter.PrinterAddress printerAddress, IDzPrinter.PrinterState printerState)
      {
        final IDzPrinter.PrinterAddress printer = printerAddress;
        switch (printerState)
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
        }
      }

      @Override
      public void onPrintProgress(IDzPrinter.PrinterAddress printerAddress, Object o, IDzPrinter.PrintProgress printProgress, Object o1)
      {
        switch (printProgress)
        {
          case Success:
            // 打印标签成功，发送通知，刷新界面提示
            mHandler.post(() -> onPrintSuccess());
            break;

          case Failed:
            // 打印标签失败，发送通知，刷新界面提示
            mHandler.post(() -> onPrintFailed());
            break;
        }
      }

      @Override
      public void onPrinterDiscovery(IDzPrinter.PrinterAddress printerAddress, IDzPrinter.PrinterInfo printerInfo)
      {

      }
    });
  }


  public void print(String assetNumber, String assetName, String assetModel)
  {
    // 调用IDzPrinter对象的getPrinterState方法获取当前打印机的连接状态
    IDzPrinter.PrinterState state = IDzPrinter.Factory.getInstance().getPrinterState();
    // 打印机未连接
    if (state == null || state.equals(IDzPrinter.PrinterState.Disconnected))
    {
      DzToast.show("打印机未连接，请先连接打印机！");

      SharedPreferences sharedPreferences = mContext.getSharedPreferences("BOP", Context.MODE_PRIVATE);
      String lastPrinterMac = sharedPreferences.getString(KeyLastPrinterMac, null);
      String lastPrinterName = sharedPreferences.getString(KeyLastPrinterName, null);
      String lastPrinterType = sharedPreferences.getString(KeyLastPrinterType, null);
      IDzPrinter.AddressType lastAddressType = DzEnum.valueOf(IDzPrinter.AddressType.class, lastPrinterType);
      if (lastPrinterMac == null || lastPrinterName == null || lastAddressType == null)
      {
        mPrinterAddress = IDzPrinter.Factory.getFirstPrinter("B11");
//      mPrinterAddress = IDzPrinter.Factory.getFirstPrinter();
      } else
      {
        mPrinterAddress = new IDzPrinter.PrinterAddress(lastPrinterName, lastPrinterMac, lastAddressType);
      }
      // 尝试连接上次成功连接的打印机
      if (mPrinterAddress != null)
      {
        if (IDzPrinter.Factory.getInstance().connect(mPrinterAddress))
        {
          // 连接打印机的请求提交成功，刷新界面提示
          onPrinterConnecting(mPrinterAddress, false);
          return;
        }
      }
      return;
    }
    // 打印机正在连接
    if (state.equals(IDzPrinter.PrinterState.Connecting))
    {
      DzToast.show("正在连接打印机，请稍后！");
      return;
    }
    // 打印机已连接
    if (printCustom(assetNumber, assetName, assetModel))
      onPrintStart();
    else
      onPrintFailed();
  }

  public void quit()
  {
    IDzPrinter.Factory.getInstance().quit();
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

  // 连接打印机请求成功提交时操作
  private void onPrinterConnecting(IDzPrinter.PrinterAddress printer, boolean showDialog)
  {
    // 连接打印机请求成功提交，刷新界面提示
    String txt = printer.shownName;
    if (TextUtils.isEmpty(txt))
      txt = printer.macAddress;
    txt = "正在连接" + '[' + txt + ']' + "打印机";
    if (showDialog)
      showStateAlertDialog(txt);
  }

  // 显示连接、打印的状态提示框
  private void showStateAlertDialog(String str)
  {
    if (stateAlertDialog != null && stateAlertDialog.isShowing())
      stateAlertDialog.setTitle(str);
    else
      stateAlertDialog = new AlertDialog.Builder(mContext).setCancelable(false).setTitle(str).show();
  }

  /**
   * 打印自定义
   */
  private boolean printCustom(String assetNumber, String assetName, String assetModel)
  {
    int pHorLen = 60 * 100;// 纸横向长度
    int pVerLen = 40 * 100;// 纸纵向长度
    int lHorLen = 57 * 100;// 线横向长度
    int lVerLen = 38 * 100;// 线纵向长度
    int lWidth = 20;// 线宽度
    int cVerLen = 8 * 100;// 单元格纵向长度
    // 创建IAtBitmap对象
    IAtBitmap api = IAtBitmap.Factory.createInstance();

    // 开始绘图任务，传入参数(页面宽度, 页面高度)
    api.startJob(pHorLen, pVerLen);
    api.setItemHorizontalAlignment(IAtBitmap.ItemAlignment.CENTER);
    api.setItemVerticalAlignment(IAtBitmap.ItemAlignment.CENTER);

    api.drawRectangle(10, 0, lHorLen, lVerLen, lWidth);

    api.drawLine(0, cVerLen, lHorLen, cVerLen, lWidth);
    api.drawLine(0, lVerLen - cVerLen, lHorLen, lVerLen - cVerLen, lWidth);
    api.drawLine(lVerLen - 2 * cVerLen, cVerLen, lVerLen - 2 * cVerLen, lVerLen - cVerLen, lWidth);
    api.drawLine(lHorLen / 2 + lVerLen / 2 - cVerLen, cVerLen, lHorLen / 2 + lVerLen / 2 - cVerLen, lVerLen - cVerLen, lWidth);
    api.drawLine(lVerLen - 2 * cVerLen, 2 * cVerLen - 2 * lWidth, lHorLen, 2 * cVerLen - 2 * lWidth, lWidth);
    api.drawLine(lVerLen - 2 * cVerLen, 3 * cVerLen - 3 * lWidth, lHorLen, 3 * cVerLen - 3 * lWidth, lWidth);

    api.drawText("聚通达资产管理", 10, 10, lHorLen - 10, cVerLen - 10, 320, IAtBitmap.FontStyle.REGULAR);
    api.draw2DQRCode(assetNumber, 110, cVerLen + 100, lVerLen - 2 * cVerLen - 200, lVerLen - 2 * cVerLen - 200);

    api.drawText("资产编号", lVerLen - 2 * cVerLen, cVerLen - lWidth, lHorLen / 2 - lVerLen / 2 + cVerLen, cVerLen - lWidth, 250, IAtBitmap.FontStyle.REGULAR);
    api.drawText(assetNumber, lHorLen / 2 + lVerLen / 2 - cVerLen, cVerLen, lHorLen / 2 - lVerLen / 2 + cVerLen, cVerLen - lWidth, 250, IAtBitmap.FontStyle.REGULAR);

    api.drawText("资产类型", lVerLen - 2 * cVerLen, 2 * cVerLen - 2 * lWidth, lHorLen / 2 - lVerLen / 2 + cVerLen, cVerLen - lWidth, 250, IAtBitmap.FontStyle.REGULAR);
    api.drawText(assetName, lHorLen / 2 + lVerLen / 2 - cVerLen, 2 * cVerLen - 2 * lWidth, lHorLen / 2 - lVerLen / 2 + cVerLen, cVerLen - lWidth, 250, IAtBitmap.FontStyle.REGULAR);

    api.drawText("品牌型号", lVerLen - 2 * cVerLen, 3 * cVerLen - 3 * lWidth, lHorLen / 2 - lVerLen / 2 + cVerLen, cVerLen - lWidth, 250, IAtBitmap.FontStyle.REGULAR);
    api.drawText(assetModel, lHorLen / 2 + lVerLen / 2 - cVerLen, 3 * cVerLen - 3 * lWidth, lHorLen / 2 - lVerLen / 2 + cVerLen, cVerLen - lWidth, 250, IAtBitmap.FontStyle.REGULAR);

    api.drawText("保护资产是一种美德", 10, lVerLen - cVerLen + 10, lHorLen - 10, cVerLen - 10, 250, IAtBitmap.FontStyle.REGULAR);

    // 结束绘图任务
    api.endJob();

    Bundle bundle = new Bundle();
//    bundle.putInt(IDzPrinter.PrintParamName.PRINT_DENSITY, printDensity);
//    bundle.putInt(IDzPrinter.PrintParamName.PRINT_SPEED, printSpeed);
//    bundle.putInt(IDzPrinter.PrintParamName.GAP_TYPE, gapType);
//    bundle.putInt(IDzPrinter.PrintParamName.PRINT_COPIES, copies);
    bundle.putInt(IDzPrinter.PrintParamName.PRINT_DIRECTION, 90);
    return IDzPrinter.Factory.getInstance().print(api, bundle);
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
    DzToast.show("标签打印成功");
  }

  // 打印请求失败或标签打印失败时操作
  private void onPrintFailed()
  {
    // 打印请求失败或标签打印失败时，刷新界面提示
    clearAlertDialog();
    DzToast.show("标签打印失败");
  }

  // 连接打印机成功时操作
  private void onPrinterConnected(IDzPrinter.PrinterAddress printer)
  {
    // 连接打印机成功时，刷新界面提示，保存相关信息
    clearAlertDialog();
    DzToast.show("连接打印机成功");
    mPrinterAddress = printer;
//    // 调用IDzPrinter对象的getPrinterInfo方法获得当前连接的打印机信息
//    String txt = getResources().getString(R.string.printer) + getResources().getString(R.string.chinesecolon);
//    txt += IDzPrinter.Factory.getInstance().getPrinterInfo().deviceName + "\n";
//    txt += IDzPrinter.Factory.getInstance().getPrinterInfo().deviceAddress;
  }

  // 连接打印机操作提交失败、打印机连接失败或连接断开时操作
  private void onPrinterDisconnected()
  {
    // 连接打印机操作提交失败、打印机连接失败或连接断开时，刷新界面提示
    clearAlertDialog();
    DzToast.show("连接打印机失败");
  }

  // 清除连接、打印的状态提示框
  private void clearAlertDialog()
  {
    if (stateAlertDialog != null && stateAlertDialog.isShowing())
      stateAlertDialog.dismiss();
    stateAlertDialog = null;
  }
}
