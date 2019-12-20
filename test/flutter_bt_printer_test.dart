import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_bt_printer/flutter_bt_printer.dart';

void main() {
  const MethodChannel channel = MethodChannel('flutter_bt_printer');

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

//  test('getPlatformVersion', () async {
//    expect(await FlutterBtPrinter.platformVersion, '42');
//  });
}
