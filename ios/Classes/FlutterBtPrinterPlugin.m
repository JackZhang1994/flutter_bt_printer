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
      //连接设备
      [[OAPrinterManager sharedInstance] connectPrinterWithSuccess:^{
          NSLog(@"连接成功");
          //开始打印
             OAAssetManageMentModel *model = [[OAAssetManageMentModel alloc]init];
             model.assetNum = @"1";
             model.assetBrand = @"2";
             model.assetModel = @"3";
             model.printerCardCount = 1;
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
