import 'package:flutter/material.dart';
import 'package:flutter_bt_printer/flutter_bt_printer.dart';

void main() => runApp(MyApp());

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Bt Printer example app'),
        ),
        body: Column(
          children: <Widget>[
//            FlatButton(
//              onPressed: () {
//                FlutterBtPrinter.init();
//              },
//              child: Text('初始化'),
//            ),
            FlatButton(
              onPressed: () {
                FlutterBtPrinter.print(
                    topTitle: '聚通达资产管理',
                    bottomTitle: '保护资产是一种美德',
                    qrCode: '2188',
                    label1Title: '资产编号',
                    label1Value: 'MacBook Air',
                    label2Title: '资产类型',
                    label2Value: 'MacBook Pro',
                    label3Title: '品牌型号',
                    label3Value: 'MacBookPro 2019');
              },
              child: Text('打印'),
            ),
            FlatButton(
              onPressed: () {
                FlutterBtPrinter.quit();
              },
              child: Text('退出'),
            ),
          ],
        ),
      ),
    );
  }
}
