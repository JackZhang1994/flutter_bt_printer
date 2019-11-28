//
//  OAAssetManageMentModel.h
//  JTDOASystem
//
//  Created by gejianmin on 2019/3/11.
//  Copyright © 2019 JuTongDa Technology. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface OAAssetManageMentModel : NSObject

@property(nonatomic,copy) NSString  * topTitle;
@property(nonatomic,copy) NSString  * bottomTitle;
@property(nonatomic,copy) NSString  * qrCode;
@property(nonatomic,copy) NSString  * label1Title;
@property(nonatomic,copy) NSString  * label1Value;
@property(nonatomic,copy) NSString  * label2Title;
@property(nonatomic,copy) NSString  * label2Value;
@property(nonatomic,copy) NSString  * label3Title;
@property(nonatomic,copy) NSString  * label3Value;

@end

NS_ASSUME_NONNULL_END
