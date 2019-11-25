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
            FlatButton(
              onPressed: () {
                FlutterBtPrinter.init();
              },
              child: Text('初始化'),
            ),
            FlatButton(
              onPressed: () {
                FlutterBtPrinter.print('2188', 'MacBook Air', 'MacBook Pro');
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
