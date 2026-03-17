/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
*/

#pragma once

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <ReplayKit/ReplayKit.h>

NS_ASSUME_NONNULL_BEGIN
#define BYTE_RTC_EXPORT __attribute__((visibility("default")))
/**
 * 
 * @type callback
 * @brief 屏幕采集相关扩展协议，仅适用于 SDK 内部采集。 <br>
 *        用户创建 extension 后生成的 SampleHandler 需继承此协议以实现屏幕共享相关能力。 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 
 */

BYTE_RTC_EXPORT @protocol ByteRtcScreenCapturerExtDelegate <NSObject>
/**
 * 
 * @type api
 * @brief 通知 Broadcast Upload Extension 停止采集屏幕并退出。
 * @note iOS 端调用 stopScreenCapture{@link #ByteRTCEngine#stopScreenCapture}，或 macOS 端调用 stopScreenVideoCapture{@link #ByteRTCEngine#stopScreenVideoCapture}，会触发该方法通知 extension 端的 SDK 停止屏幕采集。
 * @list 视频管理
 */

- (void)onQuitFromApp;
/**
 * 
 * @type api
 * @brief Socket 收到 App 侧发来的信息时，触发该回调
 * @param message App 侧发送的消息
 * @list 视频管理
 */

- (void)onReceiveMessageFromApp:(NSData *)message;
/**
 * 
 * @type api
 * @brief Socket 连接断开时触发此回调
 * @list 视频管理
 */

- (void)onSocketDisconnect;
/**
 * 
 * @type api
 * @brief Socket 连接成功时触发此回调
 * @list 视频管理
 */

- (void)onSocketConnect;
/**
 * 
 * @type api
 * @brief 检测到 App 正在进行音视频通话时触发此回调。
 * @list 视频管理
 */

- (void)onNotifyAppRunning;
@end

/**
 * 
 * @type keytype
 * @brief 屏幕采集相关扩展实例，仅适用于 SDK 内部采集。 <br>
 *        用户创建 extension 后生成的 SampleHandler 需通过此实例实现屏幕共享相关能力。
 */

BYTE_RTC_EXPORT @interface ByteRtcScreenCapturerExt : NSObject
/**
 * 
 * @type keytype
 * @brief 只读变量，用于获取 ByteRtcScreenCapturerExt 实例。
 */

@property (readonly, class) ByteRtcScreenCapturerExt *shared; 
/**
 * 
 * @type keytype
 * @brief ByteRtcScreenCapturerExt 实例的回调代理。
 */

@property (nonatomic, weak, nullable) NSObject<ByteRtcScreenCapturerExtDelegate> *delegate; 
/**
 * 
 * @type api
 * @brief 开始屏幕采集 <br>
 *        Extension 启动后，系统将自动调用该方法开启屏幕采集。
 * @param delegate 回调代理，参看 ByteRtcScreenCapturerExtDelegate{@link #ByteRtcScreenCapturerExtDelegate}
 * @param groupId App groups 中配置的 group ID
 * @list 视频管理
 */

- (void)startWithDelegate:(NSObject<ByteRtcScreenCapturerExtDelegate> *)delegate groupId:(NSString *)groupId;
/**
 * 
 * @type api
 * @brief 结束屏幕采集 <br>
 *        Extension 关闭后，系统将自动调用该方法停止屏幕采集。
 * @list 视频管理
 */

- (void)stop;
/**
 * 
 * @type api
 * @brief 推送 Extension 采集的数据
 * @param sampleBuffer 采集到的数据
 * @param sampleBufferType 数据类型
 * @list 视频管理
 */

- (void)processSampleBuffer:(CMSampleBufferRef)sampleBuffer withType:(RPSampleBufferType)sampleBufferType API_AVAILABLE(ios(10));
@end

NS_ASSUME_NONNULL_END