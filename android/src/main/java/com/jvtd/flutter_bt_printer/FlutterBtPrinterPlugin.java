package com.jvtd.flutter_bt_printer;

import android.content.Context;

import com.jvtd.flutter_bt_printer.utils.LPUtil;

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
        LPUtil.getInstance(context).init();
        result.success("success");
        break;

      case "print":
        String topTitle = call.argument("top_title");
        String bottomTitle = call.argument("bottom_title");
        String qrCode = call.argument("qr_code");
        String label1Title = call.argument("label1_title");
        String label1Value = call.argument("label1_value");
        String label2Title = call.argument("label2_title");
        String label2Value = call.argument("label2_value");
        String label3Title = call.argument("label3_title");
        String label3Value = call.argument("label3_value");
        LPUtil.getInstance(context).print(topTitle, bottomTitle, qrCode, label1Title, label1Value, label2Title, label2Value, label3Title, label3Value);
        result.success("success");
        break;

      case "quit":
        LPUtil.getInstance(context).quit();
        result.success("success");
        break;

      default:
        result.notImplemented();
        break;
    }
  }
}
