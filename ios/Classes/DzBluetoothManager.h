//
//  DzBluetoothManager.h
//  LPAPIDemo
//
//  Created by 蔡俊杰 on 2019/8/5.
//  Copyright © 2019 DothanTech. All rights reserved.
//

#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/**
 * @brief   蓝牙状态改变后的操作
 */
typedef void (^DzDidChangeBLEStateBlock)(NSInteger state);
/**
 * @brief   搜索到设备后的操作
 */
typedef void (^DzDidDiscoveredPeripheralBlock)(CBPeripheral *peripheral, NSString *realName, NSNumber *RSSI);
/**
* @brief   连接设备成功后的操作
 *
 * @param   peripheral      外部设备
 */
typedef void (^DzDidConnectedSuccessBlock)(CBPeripheral *peripheral);
/**
 * @brief   连接设备失败后的操作
 */
typedef void (^DzDidConnectedFailedBlock)(void);
/**
 * @brief   断开设备连接后的操作
 */
typedef void (^DzDidDisconnectedBlock)(NSError *error);
/**
 * @brief   接收到设备发送的信息后的操作
 *
 * @param   data            设备发送的信息
 */
typedef void (^DzDidNoticedDataBlock)(NSData *data);
/**
 * @brief   接收到设备发送的信息后的操作
 *
 * @param   text            设备发送的信息
 */
typedef void (^DzDidNoticedTextBlock)(NSString *text);
/**
 * @brief   发送数据并收到反馈后的操作
 *
 * @param   returnInfo      发送后的反馈信息
 */
typedef void (^DzDidSentDataBlock)(id returnInfo);

@interface DzBluetoothManager : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>

+ (void)didChangeBLEStateHandler:(DzDidChangeBLEStateBlock)didChangeBLEStateHandler;
+ (void)didDiscoveredPeripheralHandler:(DzDidDiscoveredPeripheralBlock)didDiscoveredPeripheralHandler;
+ (void)didNoticedDataHandler:(DzDidNoticedDataBlock)didNoticedDataHandler;
+ (void)didNoticedTextHandler:(DzDidNoticedTextBlock)didNoticedTextHandler;

/**
 * @brief   初始化蓝牙
 */
+ (void)initBLE;

/**
 * @brief   判断蓝牙是否打开
 */
+ (BOOL)isPowerOn;

/**
 * @brief   开始扫描蓝牙设备
 */
+ (void)startScanPeripherals;

/**
 * @brief   停止扫描蓝牙设备
 */
+ (void)stopScanPeripherals;

/**
 * @brief   连接蓝牙设备
 *
 * @param   peripheral              设备
 * @param   successHandler          连接成功后的操作
 * @param   failedHandler           连接失败后的操作
 * @param   disconnectHandler       连接断开后的操作
 */
+ (void)connectPeripheral:(CBPeripheral *)peripheral
           successHandler:(DzDidConnectedSuccessBlock)successHandler
            failedHandler:(DzDidConnectedFailedBlock)failedHandler
        disconnectHandler:(DzDidDisconnectedBlock)disconnectHandler;

/**
 * @brief   断开蓝牙设备连接
 *
 * @param   peripheral              设备
 * @param   disconnectedHandler     断开后的操作
 */
+ (void)disconnectPeripheral:(CBPeripheral *)peripheral
         disconnectedHandler:(DzDidDisconnectedBlock)disconnectedHandler;

/**
 * @brief   发送数据
 */
+ (void)sendData:(NSData *)data
    toPeripheral:(CBPeripheral *)peripheral
writeCharacteristic:(CBCharacteristic *)writeCharacteristic
characteristicWriteType:(CBCharacteristicWriteType)characteristicWriteType
      completion:(DzDidSentDataBlock)completion;

@end
