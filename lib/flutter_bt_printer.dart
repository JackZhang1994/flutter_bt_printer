import 'dart:async';

import 'package:flutter/services.dart';

class FlutterBtPrinter {
  static const MethodChannel _channel = const MethodChannel('flutter_bt_printer');

  static Future init() async {
    return await _channel.invokeMethod('init');
  }

  static Future print(
      {String topTitle,
      String bottomTitle,
      String qrCode,
      String label1Title,
      String label1Value,
      String label2Title,
      String label2Value,
      String label3Title,
      String label3Value}) async {
    Map<String, String> map = {
      "top_title": topTitle,
      "bottom_title": bottomTitle,
      "qr_code": qrCode,
      "label1_title": label1Title,
      "label1_value": label1Value,
      "label2_title": label2Title,
      "label2_value": label2Value,
      "label3_title": label3Title,
      "label3_value": label3Value,
    };
    return await _channel.invokeMethod('print', map);
  }

  static Future quit() async {
    return await _channel.invokeMethod('quit');
  }
}
