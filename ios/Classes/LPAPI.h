//
//  LPAPI.h
//  LPAPI
//
//  Created by 蔡俊杰 on 2019/03/27.
//  Copyright © 2011 - 2019 DothanTech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/**
 * @brief   一维码编码类型
 */
typedef NS_OPTIONS(NSUInteger, DzBarcodeMode)
{
    DzBarcodeEncodeMode_UPCA            = 20,
    DzBarcodeEncodeMode_UPCE            = 21,
    DzBarcodeEncodeMode_EAN13           = 22,
    DzBarcodeEncodeMode_EAN8            = 23,
    DzBarcodeEncodeMode_Code39          = 24,   // 4 69
    DzBarcodeEncodeMode_ITF25           = 25,   // 5 70
    DzBarcodeEncodeMode_CodaBar         = 26,
    DzBarcodeEncodeMode_Code93          = 27,
    DzBarcodeEncodeMode_Code128         = 28,   // 73
    DzBarcodeEncodeMode_ISBN            = 29,
    DzBarcodeEncodeMode_Ecode39         = 30,
    DzBarcodeEncodeMode_ITF14           = 31,
    DzBarcodeEncodeMode_ChinaPost       = 32,
    DzBarcodeEncodeMode_Matrix25        = 33,
    DzBarcodeEncodeMode_Industrial25    = 34,
};

@interface PrinterInfo : NSObject

@property (assign, nonatomic) int       deviceType;             // DEVICE_TYPE_xxx，热敏打印机/热转印打印机等等
@property (assign, nonatomic) int       deviceDPI;              // 打印精度，DPI
@property (assign, nonatomic) int       deviceWidth;            // 打印宽度，mm
@property (strong, nonatomic) NSString  *deviceName;            // 设备名称，包括型号和序列号
@property (strong, nonatomic) NSString  *deviceVersion;         // 硬件版本号
@property (strong, nonatomic) NSString  *softwareVersion;       // 软件版本号
@property (strong, nonatomic) NSString  *deviceAddress;         // MAC 地址
@property (strong, nonatomic) NSString  *manufacturer;          // 厂商名称
@property (strong, nonatomic) NSString  *seriesName;            // 产品系列名称
@property (strong, nonatomic) NSString  *devIntName;            // 内部型号名称

@property (strong, nonatomic) NSString  *hardwareFlags;         // 硬件标志位
@property (assign, nonatomic) int       softwareFlags;          // 软件标志位

@end

typedef void (^DzDidOpenedPrinterBlock)(BOOL isSuccess);

@interface LPAPI : NSObject

#pragma mark 常用接口

/**
 * @brief   设置打印机纸张间隔类型。
 *
 * @param   gapType             纸张间隔类型。0：连续纸；1：定位孔(如不支持定位孔，则调整至间隙纸)；2：间隙纸；3：黑标纸。
 */
+ (void)setPrintPageGapType:(int)gapType;

/**
 * @brief   设置打印机纸张间隔长度（跳距）。
 *
 * @param   gapLength           纸张间隔长度，单位：毫米。
 */
+ (void)setPrintPageGapLength:(int)gapLength;

/**
 * @brief   设置打印机打印浓度。
 *
 * @param   darkness            0：随打印机设置；1：最淡；6：正常；15：最浓。
 */
+ (void)setPrintDarkness:(int)darkness;

/**
 * @brief   设置打印机打印速度。
 *
 * @param   speed               0：随打印机设置；1:最慢；3：正常；5：最快。
 */
+ (void)setPrintSpeed:(int)speed;

/**
 * @brief   获取搜索到的打印机列表。
 *
 * @note    scanedPrinterNames      打印机名称列表。
 */
+ (void)scanPrinters:(void(^)(NSArray *scanedPrinterNames))completion;

/**
 * @brief   连接指定名称的打印机。
 *
 * @param   printerName     打印机名称。如果为空字符串，则打开当前客户端系统上的第一个支持的打印机。支持直接输入打印机型号。
 * @param   completion      连接打印机后的操作。
 *
 * @note    isSuccess       连接打印机是否成功。
 */
+ (void)openPrinter:(NSString *)printerName
         completion:(DzDidOpenedPrinterBlock)completion;

/**
 * @brief   获取当前连接的打印机的详细信息。
 */
+ (PrinterInfo *)connectingPrinterDetailInfos;

/**
 * @brief   关闭当前连接的打印机。
 */
+ (void)closePrinter;

/**
 * @brief   以指定的参数，开始绘制，然后调用 endDraw 结束绘制，最后调用 print 进行打印。
 *
 * @param   width           标签宽度（单位：毫米）
 * @param   height          标签高度（单位：毫米）
 * @param   orientation     标签打印方向。0：不旋转；90：顺时针旋转90度；180：旋转180度；270：逆时针旋转90度。
 */
+ (BOOL)startDraw:(double)width
           height:(double)height
      orientation:(int)orientation;

/**
 * @brief   结束绘制，同时返回打印图片。
 */
+ (UIImage *)endDraw;

/**
 * @brief   获取不带旋转方向的预览图。
 */
+ (UIImage *)previewImage;

/**
 * @brief   提交打印数据，进行真正的打印，必须先调用 endDraw 方法。
 *
 * @param   completion  打印后的操作。
 *
 * @note    isSuccess   打印结果是否成功。
 */
+ (void)print:(void(^)(BOOL isSuccess))completion;

/**
 * @brief   提交打印数据，进行真正的打印，必须先调用 endDraw 方法。
 *
 * @param   didPrintPageHandler     每打印一页后的操作。
 * @param   completion              打印后的操作。
 *
 * @note    isSuccess               打印结果是否成功。
 */
+ (void)print:(void(^)(BOOL isSuccess))didPrintPageHandler
   completion:(void(^)(BOOL isSuccess))completion;

/**
 * @brief   直接打印图片。
 *
 * @param   image                   要打印的图片。
 * @param   completion              打印后的操作。
 *
 * @note    isSuccess               打印结果是否成功。
 */
+ (void)printImage:(UIImage *)image
        completion:(void(^)(BOOL isSuccess))completion;

/**
 * @brief   设置打印元素的旋转角度。
 *
 * @param   orientation 标签打印方向。0：不旋转；90：顺时针旋转90度；180：旋转180度；270：逆时针旋转90度。
 */
+ (BOOL)setItemOrientation:(int)orientation;

/**
 * @brief   设置打印元素的水平对齐方式。
 *
 * @param   alignment   水平对齐方式。0：水平居左（默认方式）；1：水平居中；2：水平居右。
 */
+ (BOOL)setItemHorizontalAlignment:(int)alignment;

/**
 * @brief   设置打印元素的垂直对齐方式。
 *
 * @param   alignment   垂直对齐方式。0：垂直居上（默认方式）；1：垂直居中；2：垂直居下。
 */
+ (BOOL)setItemVerticalAlignment:(int)alignment;

/**
 * @brief   打印文本。
 *
 * @param   text            文本内容。
 * @param   x               打印对象的位置，单位由scaleUnit指定。
 * @param   y               打印对象的位置，单位由scaleUnit指定。
 * @param   width           打印对象的尺寸，单位由scaleUnit指定。
 * @param   height          打印对象的尺寸，单位由scaleUnit指定。
 * @param   fontHeight      文本的字体高度，单位由 scaleUnit 指定。。
 */
+ (BOOL)drawText:(NSString *)text
               x:(double)x
               y:(double)y
           width:(double)width
          height:(double)height
      fontHeight:(double)fontHeight;

/**
 * @brief   打印 Barcode 一维码。
 *
 * @param   text            一维码的内容。
 * @param   x               打印对象的位置，单位由scaleUnit指定。
 * @param   y               打印对象的位置，单位由scaleUnit指定。
 * @param   width           打印对象的尺寸，单位由scaleUnit指定。
 * @param   height          打印对象的尺寸，单位由scaleUnit指定。
 */
+ (BOOL)drawBarcode:(NSString *)text
                  x:(double)x
                  y:(double)y
              width:(double)width
             height:(double)height;

/**
 * @brief   打印 QRCode 二维码。
 *
 * @param   text            二维码的内容。
 * @param   x               打印对象的位置，单位由scaleUnit指定。
 * @param   y               打印对象的位置，单位由scaleUnit指定。
 * @param   width           打印对象的尺寸，单位由scaleUnit指定。
 */
+ (BOOL)drawQRCode:(NSString *)text
                 x:(double)x
                 y:(double)y
             width:(double)width;

/**
 * @brief   打印图像文件。支持本地文件或URL网络文件。
 *
 * @param   file        本地文件全路径名称或URL网络文件。
 * @param   threshold   图像转成打印用黑白图像的灰度阀值。
 1～255 表示灰度阀值，灰度值>=该阀值的点将被当作白点而丌打印；
 256 表示将图像转成灰度图片之后再打印，灰度点向黑白点的转换由打印机驱劢来完成；
 0/257 表示直接采用图像原始颜色进行打印，原始颜色向黑白色的转换由打印机驱劢来完成。
 对于打印二维码图片，建议灰度阀值设置为192；
 对于打印LOGO等图片，建议灰度阀值设置为0。
 */
+ (BOOL)drawImage:(NSString *)file
        threshold:(int)threshold;

/**
 * @brief   打印图像。
 *
 * @param   image       图像对象。
 * @param   threshold   图像转成打印用黑白图像的灰度阀值。
 1～255 表示灰度阀值，灰度值>=该阀值的点将被当作白点而丌打印；
 256 表示将图像转成灰度图片之后再打印，灰度点向黑白点的转换由打印机驱劢来完成；
 0/257 表示直接采用图像原始颜色进行打印，原始颜色向黑白色的转换由打印机驱劢来完成。
 对于打印二维码图片，建议灰度阀值设置为192；
 对于打印LOGO等图片，建议灰度阀值设置为0。
 */
+ (BOOL)drawImageWithImage:(UIImage *)image
                 threshold:(int)threshold;

/**
 * @brief   打印直线。
 *
 * @param   x           打印对象的位置，单位由scaleUnit指定。
 * @param   y           打印对象的位置，单位由scaleUnit指定。
 * @param   width       打印对象的尺寸，单位由scaleUnit指定。
 * @param   height      打印对象的尺寸，单位由scaleUnit指定。
 */
+ (BOOL)drawLineWithX:(double)x
                    y:(double)y
                width:(double)width
               height:(double)height;

/**
 * @brief   打印矩形。
 *
 * @param   x           打印对象的位置，单位由scaleUnit指定。
 * @param   y           打印对象的位置，单位由scaleUnit指定。
 * @param   width       打印对象的尺寸，单位由scaleUnit指定。
 * @param   height      打印对象的尺寸，单位由scaleUnit指定。
 * @param   lineWidth   线条宽度，单位由scaleUnit指定。
 * @param   isFilled    是否填充。
 */
+ (BOOL)drawRectangleWithX:(double)x
                         y:(double)y
                     width:(double)width
                    height:(double)height
                 lineWidth:(double)lineWidth
                  isFilled:(BOOL)isFilled;

/**
 * @brief   打印圆角矩形。
 *
 * @param   x           打印对象的位置，单位由scaleUnit指定。
 * @param   y           打印对象的位置，单位由scaleUnit指定。
 * @param   width       打印对象的尺寸，单位由scaleUnit指定。
 * @param   height      打印对象的尺寸，单位由scaleUnit指定。
 * @param   lineWidth   线条宽度，单位由scaleUnit指定。
 * @param   radius      圆角的半径，单位由scaleUnit指定。
 * @param   isFilled    是否填充。
 */
+ (BOOL)drawRoundRectangleWithX:(double)x
                              y:(double)y
                          width:(double)width
                         height:(double)height
                      lineWidth:(double)lineWidth
                         radius:(double)radius
                       isFilled:(BOOL)isFilled;

/**
 * @brief   打印椭圆。
 *
 * @param   x           打印对象的位置，单位由scaleUnit指定。
 * @param   y           打印对象的位置，单位由scaleUnit指定。
 * @param   width       打印对象的尺寸，单位由scaleUnit指定。
 * @param   height      打印对象的尺寸，单位由scaleUnit指定。
 * @param   lineWidth   线条宽度，单位由scaleUnit指定。
 * @param   isFilled    是否填充。
 */
+ (BOOL)drawEllipseWithX:(double)x
                       y:(double)y
                   width:(double)width
                  height:(double)height
               lineWidth:(double)lineWidth
                isFilled:(BOOL)isFilled;

/**
 * @brief   打印正圆。
 *
 * @param   x           打印对象的位置，单位由scaleUnit指定。
 * @param   y           打印对象的位置，单位由scaleUnit指定。
 * @param   width       打印对象的尺寸，单位由scaleUnit指定。
 * @param   height      打印对象的尺寸，单位由scaleUnit指定。
 * @param   lineWidth   线条宽度，单位由scaleUnit指定。
 * @param   isFilled    是否填充。
 */
+ (BOOL)drawCircleWithX:(double)x
                      y:(double)y
                  width:(double)width
                 height:(double)height
              lineWidth:(double)lineWidth
               isFilled:(BOOL)isFilled;

/**
 * @brief   打印视图。
 *
 * @param   view        视图对象。
 */
+ (BOOL)drawView:(UIView *)view;

#pragma mark 不常用接口

/**
 * @brief   通过升级文件升级打印机固件。
 *
 * @param   filePath            升级文件路径。
 */
+ (void)upgradeDeviceWithFilePath:(NSString *)filePath;

/**
 * @brief   启用进度提示框。
 */
+ (void)enableProgress:(BOOL)enable;

/**
 * @brief   开启提示回调。
 *
 101    打印机连接主动断开！
 102    当前没有连接打印机！
 103    打印机连接超时！
 104    连接打印机失败！\n可能是打印机未开机或已经被连接。
 105    打印机连接因异常被动断开！
 
 201    当前未连接打印机，\n不能进行固件更新！
 203    升级文件不存在或内容出错！\n升级失败！
 204    固件更新失败\n当前更新文件与打印机型号不匹配！
 
 0x20    当前不具备打印环境
 0x30    打印电压太低了
 0x31    打印电压太高了
 0x32    没有检测到打印头
 0x33    打印头温度太高了
 0x34    打印机盖子打开了
 0x35    没有检测到纸张
 0x36    打印头未锁紧
 0x37    未检测到碳带
 0x38    不匹配的碳带
 0x39    环境温度过低
 0x40    用完的碳带
 0x41    用完的色带
 0x42    未检测到标签纸卷
 0x43    不匹配的标签纸卷
 0x44    用完的标签纸卷
 *
 */
+ (void)didReadPrinterStateHandler:(void(^)(int code, NSString *message))didReadPrinterStateHandler;

/**
 * @brief   判断打印机名称是否支持。
 */
+ (BOOL)isPrinterSupported:(NSString *)printerName;

/**
 * @brief   设置打印机型号限制。
 *
 * @param   supportPrefixs      支持的打印机型号前缀列表。
 */
+ (void)setSupportPrefixs:(NSArray *)supportPrefixs;

/**
 * @brief   设置打印机型号限制。
 *
 * @param   unsupportPrefixs    不支持的打印机型号前缀列表。
 */
+ (void)setUnsupportPrefixs:(NSArray *)unsupportPrefixs;

/**
 * @brief   获取搜索到的打印机列表。
 *
 * @param   showList                是否显示内置搜索列表界面。
 * @param   completion              搜索完成后的操作。
 * @param   didOpenedPrinterHandler 连接打印机后的操作。
 *
 * @note    scanedPrinterNames      打印机名称列表。
 */
+ (void)scanPrinters:(BOOL)showList
        completion:(void(^)(NSArray *scanedPrinterNames))completion
        didOpenedPrinterHandler:(DzDidOpenedPrinterBlock)didOpenedPrinterHandler;

/**
 * @brief   获取当前连接的打印机名称。
 */
+ (NSString *)connectingPrinterName;

/**
 * @brief   打印文本。
 *
 * @param   text                文本内容。
 * @param   x                   打印对象的位置，单位由scaleUnit指定。
 * @param   y                   打印对象的位置，单位由scaleUnit指定。
 * @param   width               打印对象的尺寸，单位由scaleUnit指定。
 * @param   height              打印对象的尺寸，单位由scaleUnit指定。
 * @param   fontName            文本的字体名称，比如“黑体”，支持自定义字体。
 * @param   fontHeight          文本的字体高度，单位由 scaleUnit 指定。
 * @param   fontStyle           文本的打印风格。0x00：正常；0x01：粗体；0x02：斜体；0x04：下划线；0x08：删除线。
 * @param   charSpace           文本的字间距，单位由 scaleUnit 指定。
 * @param   lineSpace           文本的行间距，单位由 scaleUnit 指定。
 * @param   isAutoReturn        文本的自动换行，如果关闭，则当宽度不够时，文本将进行水平方向的压缩，高度不变。
 * @param   headIndentRanges    段落缩进位置数组。
 */
+ (BOOL)drawText:(NSString *)text
               x:(double)x
               y:(double)y
           width:(double)width
          height:(double)height
        fontName:(NSString *)fontName
      fontHeight:(double)fontHeight
       fontStyle:(int)fontStyle
       charSpace:(double)charSpace
       lineSpace:(double)lineSpace
    isAutoReturn:(BOOL)isAutoReturn
headIndentRanges:(NSArray *)headIndentRanges;

+ (BOOL)drawText:(NSString *)text
               x:(double)x
               y:(double)y
           width:(double)width
          height:(double)height
        fontName:(NSString *)fontName
      fontHeight:(double)fontHeight
       fontStyle:(int)fontStyle
       charSpace:(double)charSpace
       lineSpace:(double)lineSpace
    isAutoReturn:(BOOL)isAutoReturn;

+ (BOOL)drawText:(NSString *)text
               x:(double)x
               y:(double)y
           width:(double)width
          height:(double)height
        fontName:(NSString *)fontName
      fontHeight:(double)fontHeight
       fontStyle:(int)fontStyle
       charSpace:(double)charSpace
       lineSpace:(double)lineSpace;

+ (BOOL)drawText:(NSString *)text
               x:(double)x
               y:(double)y
           width:(double)width
          height:(double)height
      fontHeight:(double)fontHeight
       fontStyle:(int)fontStyle;

/**
 * @brief   打印紧跟上一个文本的文本，起点为上一个文本的终点。
 *
 * @param   text            文本内容。
 * @param   y               打印对象的位置，单位由scaleUnit指定。
 * @param   width           打印对象的尺寸，单位由scaleUnit指定。
 * @param   height          打印对象的尺寸，单位由scaleUnit指定。
 * @param   fontName        文本的字体名称，比如“黑体”。
 * @param   fontHeight      文本的字体高度，单位由 scaleUnit 指定。
 * @param   fontStyle       文本的打印风格。0：正常；1：粗体；2：斜体；4：下划线；8：删除线。
 * @param   charSpace       文本的字间距，单位由 scaleUnit 指定。
 * @param   lineSpace       文本的行间距，单位由 scaleUnit 指定。
 * @param   isAutoReturn    文本的自动换行，如果关闭，则当宽度不够时，文本将进行水平方向的压缩，高度不变。
 */
+ (BOOL)drawFollowText:(NSString *)text
                     y:(double)y
                 width:(double)width
                height:(double)height
              fontName:(NSString *)fontName
            fontHeight:(double)fontHeight
             fontStyle:(int)fontStyle
             charSpace:(double)charSpace
             lineSpace:(double)lineSpace
          isAutoReturn:(BOOL)isAutoReturn;

/**
 * @brief   打印 Barcode 一维码。
 *
 * @param   text            一维码的内容。
 * @param   x               打印对象的位置，单位由scaleUnit指定。
 * @param   y               打印对象的位置，单位由scaleUnit指定。
 * @param   width           打印对象的尺寸，单位由scaleUnit指定。
 * @param   height          打印对象的尺寸，单位由scaleUnit指定。
 * @param   fontName        文本的字体名称，比如“黑体”。
 * @param   fontHeight      文本的字体高度，单位由 scaleUnit 指定。
 * @param   fontStyle       文本的打印风格。0：正常；1：粗体；2：斜体；4：下划线；8：删除线。
 * @param   textFlag        文本的位置。0：无文字；1：条码上方；2：条码下方。
 * @param   encodeMode      编码模式。
 * @param   autoResetWidth  启动宽度自动调整。
 */
+ (BOOL)drawBarcode:(NSString *)text
                  x:(double)x
                  y:(double)y
              width:(double)width
             height:(double)height
           fontName:(NSString *)fontName
         fontHeight:(double)fontHeight
          fontStyle:(int)fontStyle
           textFlag:(NSUInteger)textFlag
         encodeMode:(DzBarcodeMode)encodeMode
     autoResetWidth:(BOOL)autoResetWidth;

/**
 * @brief   打印 QRCode 二维码。
 *
 * @param   text            二维码的内容。
 * @param   x               打印对象的位置，单位由scaleUnit指定。
 * @param   y               打印对象的位置，单位由scaleUnit指定。
 * @param   width           打印对象的尺寸，单位由scaleUnit指定。
 * @param   height          打印对象的尺寸，单位由scaleUnit指定。
 * @param   eccLevel        二维码纠错级别，0：L；1：M；2：Q；3：H。
 * @param   autoResetWidth  启动宽度自动调整。
 */
+ (BOOL)drawQRCode:(NSString *)text
                 x:(double)x
                 y:(double)y
             width:(double)width
            height:(double)height
          eccLevel:(NSUInteger)eccLevel
    autoResetWidth:(BOOL)autoResetWidth;

/**
 * @brief   打印图像文件。支持本地文件或URL网络文件。
 *
 * @param   file        本地文件全路径名称或URL网络文件。
 * @param   x           打印对象的位置，单位由scaleUnit指定。
 * @param   y           打印对象的位置，单位由scaleUnit指定。
 * @param   width       打印对象的尺寸，单位由scaleUnit指定。
 * @param   height      打印对象的尺寸，单位由scaleUnit指定。
 * @param   threshold   图像转成打印用黑白图像的灰度阀值。
 1～255 表示灰度阀值，灰度值>=该阀值的点将被当作白点而丌打印；
 256 表示将图像转成灰度图片之后再打印，灰度点向黑白点的转换由打印机驱劢来完成；
 0/257 表示直接采用图像原始颜色进行打印，原始颜色向黑白色的转换由打印机驱劢来完成。
 对于打印二维码图片，建议灰度阀值设置为192；
 对于打印LOGO等图片，建议灰度阀值设置为0。
 */
+ (BOOL)drawImage:(NSString *)file
                x:(double)x
                y:(double)y
            width:(double)width
           height:(double)height
        threshold:(int)threshold;

/**
 * @brief   打印图像。
 *
 * @param   image       图像对象。
 * @param   x           打印对象的位置，单位由scaleUnit指定。
 * @param   y           打印对象的位置，单位由scaleUnit指定。
 * @param   width       打印对象的尺寸，单位由scaleUnit指定。
 * @param   height      打印对象的尺寸，单位由scaleUnit指定。
 * @param   threshold   图像转成打印用黑白图像的灰度阀值。
 1～255 表示灰度阀值，灰度值>=该阀值的点将被当作白点而丌打印；
 256 表示将图像转成灰度图片之后再打印，灰度点向黑白点的转换由打印机驱劢来完成；
 0/257 表示直接采用图像原始颜色进行打印，原始颜色向黑白色的转换由打印机驱劢来完成。
 对于打印二维码图片，建议灰度阀值设置为192；
 对于打印LOGO等图片，建议灰度阀值设置为0。
 */
+ (BOOL)drawImageWithImage:(UIImage *)image
                         x:(double)x
                         y:(double)y
                     width:(double)width
                    height:(double)height
                 threshold:(int)threshold;

/**
 * @brief   打印视图。
 *
 * @param   view        视图对象。
 * @param   x           打印对象的位置，单位由scaleUnit指定。
 * @param   y           打印对象的位置，单位由scaleUnit指定。
 * @param   width       打印对象的尺寸，单位由scaleUnit指定。
 * @param   height      打印对象的尺寸，单位由scaleUnit指定。
 */
+ (BOOL)drawView:(UIView *)view
               x:(double)x
               y:(double)y
           width:(double)width
          height:(double)height;

@end
