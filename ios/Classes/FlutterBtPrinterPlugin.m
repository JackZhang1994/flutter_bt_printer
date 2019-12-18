#import "FlutterBtPrinterPlugin.h"
#import "OAPrinterManager.h"

@implementation FlutterBtPrinterPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  FlutterMethodChannel* channel = [FlutterMethodChannel
      methodChannelWithName:@"flutter_bt_printer"
            binaryMessenger:[registrar messenger]];
  FlutterBtPrinterPlugin* instance = [[FlutterBtPrinterPlugin alloc] init];
  [registrar addMethodCallDelegate:instance channel:channel];
}

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
  
    if ([@"init" isEqualToString:call.method]) {
    //初始化
    OAPrinterManager *printManager =  [OAPrinterManager sharedInstance];
      
  }else if ([@"print" isEqualToString:call.method]){
      NSDictionary *argumentDic = call.arguments;
      NSString *topTitle = argumentDic[@"top_title"];
      NSString *bottomTitle = argumentDic[@"bottom_title"];
      NSString *qrCode = argumentDic[@"qr_code"];
      NSString *label1Title = argumentDic[@"label1_title"];
      NSString *label1Value = argumentDic[@"label1_value"];
      NSString *label2Title = argumentDic[@"label2_title"];
      NSString *label2Value = argumentDic[@"label2_value"];
      NSString *label3Title = argumentDic[@"label3_title"];
      NSString *label3Value = argumentDic[@"label3_value"];
      
      OAAssetManageMentModel *model = [[OAAssetManageMentModel alloc]init];
      model.topTitle = topTitle;
      model.bottomTitle = bottomTitle;
      model.qrCode = qrCode;
      model.label1Title = label1Title;
      model.label1Value = label1Value;
      model.label2Title = label2Title;
      model.label2Value = label2Value;
      model.label3Title = label3Title;
      model.label3Value = label3Value;
      
      //连接设备
      [[OAPrinterManager sharedInstance] connectPrinterWithSuccess:^{
          NSLog(@"连接成功");
          //开始打印
             [[OAPrinterManager sharedInstance] printWithModel:model success:^{
                 NSLog(@"打印成功");
             } failure:^{
                 NSLog(@"打印失败");
             }];
      } failure:^{
          NSLog(@"连接失败");
      }];
      
  }else if ([@"quit" isEqualToString:call.method]){
      
      
  }else {
    result(FlutterMethodNotImplemented);
  }
    
    
}

@end
