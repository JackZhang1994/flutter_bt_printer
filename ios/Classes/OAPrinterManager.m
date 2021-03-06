//
//  OAPrinterManager.m
//  JTDOASystem
//
//  Created by gejianmin on 2019/3/11.
//  Copyright © 2019 JuTongDa Technology. All rights reserved.
//

#import "OAPrinterManager.h"
#import "LPAPI.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


/** 打印机名称*/
#define  printerName @"B11-81123477"

@interface OAPrinterManager()

@property(nonatomic,strong) UIImage  * printLabelImage;
@property(nonatomic,copy)OAPrinterSuccess  printerSuccess;
@property(nonatomic,copy)OAPrinterFailed  printerFailed;
@property(nonatomic,strong)OAPrinterConnectSuccess  printerConnectSuccess;
@property(nonatomic,strong)OAPrinterConnectFailed  printerConnectFailed;
@property(nonatomic,assign)int  printerCount;

@end

@implementation OAPrinterManager

static OAPrinterManager * manager = nil;
static dispatch_once_t onceToken;


+ (instancetype)sharedInstance{
    dispatch_once(&onceToken, ^{
        manager=[[OAPrinterManager alloc] init];
    });
    return manager;
}
- (id)init {
    self = [super init];
    if (self) {
        [LPAPI enableProgress:NO];
        self.printLabelImage = nil;
        self.printerCount = 1;
    }
    return self;
}
- (void)printWithModel:(OAAssetManageMentModel *)model success:(OAPrinterSuccess )success failure:(OAPrinterFailed)failure{

    self.printerSuccess = success;
    self.printerFailed = failure;
    
   
        __weak typeof(self) weakSelf = self;
        [self drawCardWithModel:model];
        [LPAPI print:^(BOOL isSuccess){
            if (isSuccess)
            {
                weakSelf.printerSuccess();
            }else{
                weakSelf.printerFailed();
            }
        }];
    
}
-(void)drawCardWithModel:(OAAssetManageMentModel *)model{
    CGFloat labelWidth = 57.0f;
    CGFloat labelHeight = 38.0f;
    CGFloat orignX = 0.5f;
    CGFloat orignY = 7.6f;
    
    // 开始绘制
    [LPAPI startDraw:labelWidth
              height:labelHeight
         orientation:90];
    
    // 矩形
    [LPAPI drawRectangleWithX:0.0f
                            y:0.0f
                        width:labelWidth
                       height:labelHeight
                    lineWidth:0.5f
                     isFilled:NO];
    //  /*
    // 横线1
    [LPAPI drawLineWithX:orignX
                       y:orignY
                   width:labelWidth
                  height:0.5f];
    // 横线2
    [LPAPI drawLineWithX:(orignY*3)
                       y:orignY*2
                   width:labelWidth
                  height:0.5f];
    // 横线3
    [LPAPI drawLineWithX:(orignY*3)
                       y:orignY*3
                   width:labelWidth
                  height:0.5f];
    //横线4
    [LPAPI drawLineWithX:0.5f
                       y:labelHeight-orignY
                   width:labelWidth
                  height:0.5f];
    //竖线1
    [LPAPI drawLineWithX:orignY *3
                       y:orignY
                   width:0.5
                  height:orignY*3+0.5];
    //竖线2
    [LPAPI drawLineWithX:(labelWidth-(orignY*3))/2+orignY *3
                       y:orignY
                   width:0.5
                  height:orignY*3+0.5];
    
    //二维码
    [LPAPI drawQRCode:model.qrCode
                    x:2.4
                    y:10
                width:18];
    //标题
    [LPAPI setItemHorizontalAlignment:1];
    [LPAPI drawText:model.topTitle
                  x:15
                  y:2
              width:30
             height:5
         fontHeight:3];
    //资产编号
    [LPAPI drawText:model.label1Title
                  x:orignY*3 //((labelWidth-orignY*3)/2-10)/2
                  y:orignY+2.5
              width:(labelWidth-orignY*3)/2
             height:5
         fontHeight:2.5];
    [LPAPI drawText:model.label1Value
                  x:orignY*3 + (labelWidth-orignY*3)/2
                  y:orignY+2.5
              width:(labelWidth-orignY*3)/2
             height:5
         fontHeight:2.5];
    //资产名称
    [LPAPI drawText:model.label2Title
                  x:orignY*3
                  y:orignY*2+2.5
              width:(labelWidth-orignY*3)/2
             height:5
         fontHeight:2.5];
    [LPAPI drawText:model.label2Value
                  x:orignY*3 + (labelWidth-orignY*3)/2
                  y:orignY*2+2.5
              width:(labelWidth-orignY*3)/2
             height:5
         fontHeight:2.5];
    //规格型号
    [LPAPI drawText:model.label3Title
                  x:orignY*3
                  y:orignY*3+2.5
              width:(labelWidth-orignY*3)/2
             height:5
         fontHeight:2.5];
    [LPAPI drawText:model.label3Value
                  x:orignY*3+(labelWidth-orignY*3)/2
                  y:orignY*3+2.5
              width:(labelWidth-orignY*3)/2
             height:5
         fontHeight:2.5];
    //提示信息
    [LPAPI drawText:model.bottomTitle
                  x:15
                  y:9+9+9+6
              width:30
             height:5
         fontHeight:2.5];
    
    self.printLabelImage = [LPAPI endDraw];
}
// 直接连接指定的打印机
- (void)connectPrinterWithSuccess:(OAPrinterConnectSuccess)success failure:(OAPrinterConnectFailed)failure{
    self.printerConnectSuccess = success;
    self.printerConnectFailed = failure;
    PrinterInfo * model = [LPAPI connectingPrinterDetailInfos];/** 打印机详情*/
    if (model.deviceName.length !=0) {/** 已连接打印机，不需要再次连接*/
        self.printerConnectSuccess();
        return;
    }else{
        __weak typeof(self) weakSelf = self;
        // 直接指定打印机名称
        [LPAPI openPrinter:printerName completion:^(BOOL isSuccess){
            if (isSuccess){
                weakSelf.printerConnectSuccess();
            }else{
                weakSelf.printerConnectFailed();
            }
        }];
    }
}

 
- (void)closePrinter{
    
    [LPAPI closePrinter];
}

@end
