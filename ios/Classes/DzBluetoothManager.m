//
//  DzBluetoothManager.m
//  LPAPIDemo
//
//  Created by 蔡俊杰 on 2019/8/5.
//  Copyright © 2019 DothanTech. All rights reserved.
//

#import "DzBluetoothManager.h"

static DzDidChangeBLEStateBlock           didChangeBLEStateBlock;                // 蓝牙状态改变后的操作
static DzDidDiscoveredPeripheralBlock     didDiscoveredPeripheralBlock;          // 搜索到设备后的操作
static DzDidConnectedSuccessBlock         didConnectedSuccessBlock;              // 连接设备成功后的操作
static DzDidConnectedFailedBlock          didConnectedFailedBlock;               // 连接设备失败后的操作
static DzDidDisconnectedBlock             didDisconnectedBlock;                  // 断开设备连接后的操作
static DzDidDisconnectedBlock             didDisconnectedWhenConBlock;           // 断开设备连接后的操作
static DzDidNoticedDataBlock              didNoticedDataBlock;                   // 接收到设备发送的信息后的操作
static DzDidNoticedTextBlock              didNoticedTextBlock;                   // 接收到设备发送的信息后的操作
static DzDidSentDataBlock                 didSentDataBlock;                      // 发送数据后的操作

@implementation DzBluetoothManager

+ (void)didChangeBLEStateHandler:(DzDidChangeBLEStateBlock)didChangeBLEStateHandler
{
    didChangeBLEStateBlock = nil;
    didChangeBLEStateBlock = didChangeBLEStateHandler;
}

+ (void)didDiscoveredPeripheralHandler:(DzDidDiscoveredPeripheralBlock)didDiscoveredPeripheralHandler
{
    didDiscoveredPeripheralBlock = nil;
    didDiscoveredPeripheralBlock = didDiscoveredPeripheralHandler;
}

+ (void)didNoticedDataHandler:(DzDidNoticedDataBlock)didNoticedDataHandler
{
    didNoticedDataBlock = nil;
    didNoticedDataBlock = didNoticedDataHandler;
}

+ (void)didNoticedTextHandler:(DzDidNoticedTextBlock)didNoticedTextHandler
{
    didNoticedTextBlock = nil;
    didNoticedTextBlock = didNoticedTextHandler;
}

+ (instancetype)shared
{
    static id shared = nil;
    static dispatch_once_t once;
    dispatch_once(&once, ^
                  {
                      shared = [[self alloc] init];
                  });
    
    return shared;
}

// 蓝牙广播中心
+ (CBCentralManager *)centralManager
{
    static CBCentralManager *centralManager = nil;
    if (centralManager == nil)
    {
        dispatch_async(dispatch_get_main_queue(), ^
                       {
                           centralManager = [[CBCentralManager alloc] 
                                             initWithDelegate:[self shared]
                                             queue:nil
                                             options:@{CBPeripheralManagerOptionShowPowerAlertKey : @YES}];
                       });
    }
    
    return centralManager;
}

// 初始化蓝牙
+ (void)initBLE
{
    [self centralManager];
}

+ (BOOL)isPowerOn
{
    return [[self centralManager] state] == CBCentralManagerStatePoweredOn;
}

// 开始扫描蓝牙设备
+ (void)startScanPeripherals
{
    [self refreshPeripherals];
}

+ (void)refreshPeripherals
{
    @synchronized (self) 
    {        
        if ([[self centralManager] state] == CBCentralManagerStatePoweredOn)
        {            
            //NSLog(@"开始扫描蓝牙设备......");
            NSDictionary *dictOptions = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:false], CBCentralManagerScanOptionAllowDuplicatesKey, nil];
            [[self centralManager] stopScan];
            [[self centralManager] scanForPeripheralsWithServices:nil
                                                          options:dictOptions];
        }
        else
        {
            [self centralManager];
        }
    }
}

// 停止扫描蓝牙设备
+ (void)stopScanPeripherals
{        
    dispatch_async(dispatch_get_main_queue(), ^
                   {
                       if ([[self centralManager] state] == CBCentralManagerStatePoweredOn)
                       {
                           //NSLog(@"停止扫描蓝牙设备");
                           [[self centralManager] stopScan];
                       }
                   });
}

// 连接蓝牙设备
+ (void)connectPeripheral:(CBPeripheral *)peripheral
           successHandler:(DzDidConnectedSuccessBlock)successHandler
            failedHandler:(DzDidConnectedFailedBlock)failedHandler
        disconnectHandler:(DzDidDisconnectedBlock)disconnectHandler
{
    didConnectedSuccessBlock    = nil;
    didConnectedSuccessBlock    = successHandler;
    didConnectedFailedBlock     = nil;
    didConnectedFailedBlock     = failedHandler;
    didDisconnectedWhenConBlock = nil;
    didDisconnectedWhenConBlock = disconnectHandler;
    
    dispatch_async(dispatch_get_main_queue(), ^
                   {
                       [[self centralManager] connectPeripheral:peripheral
                                                        options:nil];
                   });
}

// 断开蓝牙设备
+ (void)disconnectPeripheral:(CBPeripheral *)peripheral
         disconnectedHandler:(DzDidDisconnectedBlock)disconnectedHandler
{
    didDisconnectedBlock = nil;
    didDisconnectedBlock = disconnectedHandler;
    
    dispatch_async(dispatch_get_main_queue(), ^
                   {
                       [[self centralManager] cancelPeripheralConnection:peripheral];
                   });
}

// 发送数据
+ (void)sendData:(NSData *)data
        toPeripheral:(CBPeripheral *)peripheral
        writeCharacteristic:(CBCharacteristic *)writeCharacteristic
        characteristicWriteType:(CBCharacteristicWriteType)characteristicWriteType
        completion:(DzDidSentDataBlock)completion
{
    dispatch_async(dispatch_get_main_queue(), ^
                   {
                       didSentDataBlock = completion;
                       if ([[self centralManager] state] == CBCentralManagerStatePoweredOn)
                       {
                           // 蓝牙已启动
                           if ([peripheral state] == CBPeripheralStateConnected)
                           {
                               [peripheral writeValue:data
                                    forCharacteristic:writeCharacteristic
                                                 type:characteristicWriteType];
                           }
                       }
                   });
}

#pragma mark Delegate
#pragma mark CBCentralManagerDelegate
- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    // 蓝牙状态改变
    //-----------------------------------------------------------------------------------------------------
    if (didChangeBLEStateBlock != nil)
    {
        didChangeBLEStateBlock([central state]);
    }
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI
{
    // 发现一个蓝牙设备后的操作
    //-----------------------------------------------------------------------------------------------------    
    @synchronized (self) 
    {
        NSString *strName = [advertisementData objectForKey:@"kCBAdvDataLocalName"];     
        strName = strName ? strName : [peripheral name];
        
        if (didDiscoveredPeripheralBlock != nil)
        {
            didDiscoveredPeripheralBlock(peripheral, strName, RSSI);
        }
    }
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    // 连接上蓝牙设备后的操作
    //-----------------------------------------------------------------------------------------------------
    //NSLog(@"已连接蓝牙设备");
    [peripheral setDelegate:[DzBluetoothManager shared]];
    [peripheral discoverServices:nil];
    
    if (didConnectedSuccessBlock != nil)
    {
        didConnectedSuccessBlock(peripheral);
    }
}

- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    // 与设备连接失败后的处理
    //-----------------------------------------------------------------------------------------------------
    //NSLog(@"连接蓝牙设备失败，原因：%@", [error localizedDescription]);    
    if (didConnectedFailedBlock != nil)
    {
        didConnectedFailedBlock();
    }
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    // 断开与设备连接后的处理
    //-----------------------------------------------------------------------------------------------------
    //NSLog(@"已断开蓝牙设备连接");
    if (didDisconnectedBlock != nil)
    {
        didDisconnectedBlock(error);
    }
    
    if (didDisconnectedWhenConBlock != nil)
    {
        didDisconnectedWhenConBlock(error);
    }
}

#pragma mark CBPeripheralDelegate
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error
{
    // 查询到蓝牙服务后的处理
    //-----------------------------------------------------------------------------------------------------
    
    
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error
{
    // 查询到服务特征值后的处理
    //-----------------------------------------------------------------------------------------------------
    
    
}

- (void)peripheral:(CBPeripheral *)peripheral didReadRSSI:(NSNumber *)RSSI error:(NSError *)error
{
    // 读取到设备信号强度后的处理
    //-----------------------------------------------------------------------------------------------------
    
    
}

- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    // 接收到设备发送来的数据后的处理
    //-----------------------------------------------------------------------------------------------------
    if (error != nil)
    {
        NSLog(@"接收到蓝牙设备发送来的数据失败，原因：%@", [error localizedDescription]);
    }
    else
    {        
        if ([[[characteristic UUID] UUIDString] isEqualToString:@"2A24"])
        {
            // 读取设备信息
            NSString *str = [[NSString alloc] initWithData:[characteristic value] encoding:NSUTF8StringEncoding];
            
            //NSLog(@"2A24----：%@", str);
            
            if (didNoticedTextBlock != nil)
            {
                didNoticedTextBlock(str);
            }
        }
        else
        {
            //NSLog(@"----：%@", [characteristic value]);
            
            if (didNoticedDataBlock != nil)
            {
                didNoticedDataBlock([characteristic value]);
            }
        }
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    // 发送数据到设备后的处理
    //-----------------------------------------------------------------------------------------------------
    if (error != nil)
    {
        NSLog(@"发送数据到蓝牙设备失败，原因：%@", [error localizedDescription]);
    }
    else
    {
        if (didSentDataBlock != nil)
        {
            didSentDataBlock([characteristic value]);
        }
    }
}

@end
