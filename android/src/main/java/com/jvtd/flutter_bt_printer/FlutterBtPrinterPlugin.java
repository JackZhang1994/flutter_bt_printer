package com.jvtd.flutter_bt_printer;

import android.content.Context;
import android.content.SharedPreferences;

import com.dothantech.common.DzEnum;
import com.dothantech.common.DzToast;
import com.dothantech.printer.IDzPrinter;
import com.jvtd.flutter_bt_printer.utils.PrintUtil;

import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;

/**
 * FlutterBtPrinterPlugin
 */
public class FlutterBtPrinterPlugin implements MethodCallHandler
{
  private Context context;

  private FlutterBtPrinterPlugin(Context context)
  {
    this.context = context;
  }

  /**
   * Plugin registration.
   */
  public static void registerWith(Registrar registrar)
  {
    final MethodChannel channel = new MethodChannel(registrar.messenger(), "flutter_bt_printer");
    channel.setMethodCallHandler(new FlutterBtPrinterPlugin(registrar.context()));
  }

  @Override
  public void onMethodCall(MethodCall call, Result result)
  {
    String method = call.method;
    switch (method)
    {
      case "init":
        // 初始化 IDzPrinter 对象(简单起见，不处理结果通知)
        PrintUtil.getInstance(context).init();
        result.success("success");
        break;

      case "print":
        String assetNumber = call.argument("asset_number");
        String assetName = call.argument("asset_name");
        String assetModel = call.argument("asset_model");
        PrintUtil.getInstance(context).print(assetNumber, assetName, assetModel);
        result.success("success");
        break;

      case "quit":
        // 断开蓝牙连接，释放 IDzPrinter 对象
        PrintUtil.getInstance(context).quit();
        result.success("success");
        break;

      default:
        result.notImplemented();
        break;
    }
  }
}
