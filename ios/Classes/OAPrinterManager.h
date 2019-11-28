//
//  OAPrinterManager.h
//  JTDOASystem
//
//  Created by gejianmin on 2019/3/11.
//  Copyright © 2019 JuTongDa Technology. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OAAssetManageMentModel.h"
NS_ASSUME_NONNULL_BEGIN

typedef void(^OAPrinterSuccess) ();
typedef void(^OAPrinterFailed) ();

typedef void(^OAPrinterConnectSuccess) ();
typedef void(^OAPrinterConnectFailed) ();

@interface OAPrinterManager : NSObject

+ (instancetype)sharedInstance;

/**
 打印机链接状态
 
 @param success 连接成功
 @param failure 连接失败
 */
- (void)connectPrinterWithSuccess:(OAPrinterConnectSuccess )success failure:(OAPrinterConnectFailed)failure;
/**
 打印机打印

 @param model 数据模型
 @param success 打印成功
 @param failure 打印失败
 */
- (void)printWithModel:(OAAssetManageMentModel *)model success:(OAPrinterSuccess )success failure:(OAPrinterFailed)failure;

/**
    关闭打印机
 */
- (void)closePrinter;
@end

NS_ASSUME_NONNULL_END
