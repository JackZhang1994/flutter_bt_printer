//
//  OAAssetManageMentModel.h
//  JTDOASystem
//
//  Created by gejianmin on 2019/3/11.
//  Copyright © 2019 JuTongDa Technology. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
/*
 "assetBrand": "苹果",
 "assetDate": "2019-02-19 00:00:00",
 "deptName": "OA技术部",
 "empName": "王方超",
 "assetNum": "22",
 "assetModel": "TYY_01",
 "assetClass": "服务器",
 "assetStatus": "使用中"
 */
@interface OAAssetManageMentModel : NSObject

@property(nonatomic,copy) NSString  * assetBrand;          /** 资产名称*/
@property(nonatomic,copy) NSString  * assetDate;           /** 资产使用时间*/
@property(nonatomic,copy) NSString  * assetNum;            /** 资产ID*/
@property(nonatomic,copy) NSString  * deptName;            /** 资产所属部门*/
@property(nonatomic,copy) NSString  * empName;             /** 资产使用人*/
@property(nonatomic,copy) NSString  * assetModel;          /** 资产型号*/
@property(nonatomic,copy) NSString  * assetClass;          /** 资产所属类别*/
@property(nonatomic,copy) NSString  * assetStatus;         /** 资产使用状态*/
@property(nonatomic,assign) NSInteger  printerCardCount;   /** 资产打印张数*/
@property(nonatomic,strong) NSString  *  extraValue;       /** 扩展字段*/

@end

NS_ASSUME_NONNULL_END
