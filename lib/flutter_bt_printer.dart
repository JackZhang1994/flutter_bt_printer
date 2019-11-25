import 'dart:async';

import 'package:flutter/services.dart';

class FlutterBtPrinter {
  static const MethodChannel _channel = const MethodChannel('flutter_bt_printer');

  static Future init() async {
    return await _channel.invokeMethod('init');
  }

  static Future print(String assetNumber, String assetName, String assetModel) async {
    Map<String, String> map = {"asset_number": assetNumber, "asset_name": assetName, "asset_model": assetModel};
    return await _channel.invokeMethod('print', map);
  }

  static Future quit() async {
    return await _channel.invokeMethod('quit');
  }
}
