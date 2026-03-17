#import "ByteRTCEngine.h"
#import "rtc/ByteRTCVideoDefinesEx.h"

@class ByteRTCEngineEx;
@class ByteRTCRoomEx;

/**
 * 
 * @type callback
 * @brief ByteRTCEngineDelegateEx Class<br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 */
@protocol ByteRTCEngineDelegateEx <NSObject>
@optional











































































































@end
/**
 * 
 * @type api
 * @brief ByteRTCEngineEx class
 */
BYTERTC_APPLE_EXPORT @interface ByteRTCEngineEx : ByteRTCEngine
/**
* 
* @type api
* @brief 创建 ByteRTCEngineEx 实例。  <br>
*        如果当前进程中未创建引擎实例，那么你必须先使用此方法，以使用 RTC 提供的各种音视频能力。  <br>
*        如果当前进程中已创建了引擎实例，再次调用此方法时，会返回已创建的引擎实例。
* @param appId 每个应用的唯一标识符。只有使用相同的 app_id 生成的实例，才能够进行音视频通信。
* @param delegate SDK 回调给应用层的 Callback 对象，详见 ByteRTCEngineDelegate{@link #ByteRTCEngineDelegate} 。
* @param delegateEx SDK 回调给应用层的 Callback 对象，详见 ByteRTCEngineDelegateEx{@link #ByteRTCEngineDelegateEx} 。
* @param parameters 私有参数。如需使用请联系技术支持人员。
* @return ByteRTCVideoEx：创建成功。返回一个可用的 ByteRTCEngineEx{@link #ByteRTCEngineEx} 实例  
*/
+ (ByteRTCEngineEx* _Nullable)createRTCEngineEx:(ByteRTCEngineConfig * _Nonnull) config
                                     delegate:(id<ByteRTCEngineDelegate> _Nullable)delegate;

/**
* 
* @type api
* @brief 创建 ByteRTCEngineEx 实例。  <br>
*        如果当前进程中未创建引擎实例，那么你必须先使用此方法，以使用 RTC 提供的各种音视频能力。  <br>
*        如果当前进程中已创建了引擎实例，再次调用此方法时，会返回新创建的引擎实例。
* @param appId 每个应用的唯一标识符。只有使用相同的 app_id 生成的实例，才能够进行音视频通信。
* @param delegate SDK 回调给应用层的 Callback 对象，详见 ByteRTCEngineDelegate{@link #ByteRTCEngineDelegate} 。
* @param delegateEx SDK 回调给应用层的 Callback 对象，详见 ByteRTCEngineDelegateEx{@link #ByteRTCEngineDelegateEx} 。
* @param parameters 私有参数。如需使用请联系技术支持人员。
* @return ByteRTCVideoEx：创建成功。返回一个可用的 ByteRTCEngineEx{@link #ByteRTCEngineEx} 实例
*/
+ (ByteRTCEngine* _Nullable)createRTCEngineExMulti:(ByteRTCEngineConfig* _Nonnull)config
                                          delegate:(id<ByteRTCEngineDelegate> _Nullable)delegate;

/**
 * 
 * @type api
 * @brief 销毁由 ByteRTCEngineEx 实例，并释放所有相关资源。
 * @note  
 *        + 请确保和需要销毁的 ByteRTCEngineEx 实例相关的业务场景全部结束后，才调用此方法。如果在多线程场景下，调用此接口后，又调用了其他 ByteRTCEngineEx 相关接口，可能导致 SDK 崩溃。该方法在调用之后，会销毁所有和此 ByteRTCEngineEx 实例相关的内存，并且停止与媒体服务器的任何交互。  <br>
 *        + 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，会导致死锁。同时此方法是耗时操作，不建议在主线程调用本方法，避免主线程阻塞。
 */
+ (void)destroyRTCEngineEx NS_SWIFT_NAME(destroyRTCVideoEx());


/**
 * 
 * @type api
 * @brief 销毁由createRTCEngineExMulti:delegate:parameters::delegateEx:parameters{@link #ByteRTCEngineEx#createRTCEngineExMulti:delegate:parameters:delegateEx:parameters}  ByteRTCEngineEx 实例，并释放所有相关资源。
 * @note
 *        + 请确保和需要销毁的 ByteRTCEngineEx 实例相关的业务场景全部结束后，才调用此方法。如果在多线程场景下，调用此接口后，又调用了其他 ByteRTCEngineEx 相关接口，可能导致 SDK 崩溃。该方法在调用之后，会销毁所有和此 ByteRTCEngineEx 实例相关的内存，并且停止与媒体服务器的任何交互。  <br>
 *        + 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，会导致死锁。同时此方法是耗时操作，不建议在主线程调用本方法，避免主线程阻塞。
 */
+ (void)destroyRTCEngineExMulti:(ByteRTCEngineEx*)engine;
/**
 * 
 * @type api
 * @brief 设置回调句柄。
 * @param delegateEx 参看 ByteRTCEngineDelegateEx{@link #ByteRTCEngineDelegateEx}。
 */
- (void)setDelegateEx:(id<ByteRTCEngineDelegateEx> _Nullable)delegateEx;
/**
 * 
 * @type api
 * @brief 创建 ByteRTCRoomEx{@link #ByteRTCRoomEx} 实例。<br>
 *        调用此方法仅返回一个房间实例，你仍需调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 才能真正地创建/加入房间。
 * @param roomId 标识通话房间的房间 ID。该字符串符合正则表达式：`[a-zA-Z0-9_@\-\.]{1,128}`。
 * @return 创建的 ByteRTCRoomEx{@link #ByteRTCRoomEx} 房间实例。
 * @note 请勿使用同样的 roomId 创建多个房间，否则后创建的房间实例会替换先创建的房间实例。
 */
- (ByteRTCRoomEx* _Nullable)createRTCRoomEx:(NSString* _Nonnull)roomId;
/**
 * 
 * @type api
 * @brief 设置视频源的相关配置。
 * @param streamIndex 源对应的流的索引。参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param config 参看 ByteRTCVideoSourceConfig{@link #ByteRTCVideoSourceConfig}。
 * @return  
 *        + 0: 调用成功。<br>
 *        + < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 */
- (int)setVideoSource:(ByteRTCStreamIndex)streamIndex videoSourceConfig:(ByteRTCVideoSourceConfig* _Nonnull)config;
/**
 * 
 * @type api
 * @region 自定义流处理
 * @brief 推送外部视频帧。
 * @param streamIndex 源对应的流的索引。参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param frame 该视频帧包含待 SDK 编码的视频数据，参考 ByteRTCVideoFrame{@link #ByteRTCVideoFrame}。
 * @return 方法调用结果：<br>
 *        + 0：成功；<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 * @note 推送外部视频帧前，必须调用 setVideoSource:videoSourceConfig:{@link #ByteRTCEngineEx#setVideoSource:videoSourceConfig:} 开启外部视频源采集。
 */
- (int)pushExternalVideoFrame:(ByteRTCStreamIndex)streamIndex videoFrame:(ByteRTCVideoFrameData* _Nonnull)frame;
/**
 * 
 * @type api
 * @brief 设置流的优先级。<br>
 *        在网络条件有限的情况下，优先保障高优先级视频流的清晰度和流畅度，优先保障高优先级流的音频流畅度和音质。
 * @param streamIndex 源对应的流的索引。参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param priority 参看 ByteRTCStreamPriority{@link #ByteRTCStreamPriority}。
 * @return 方法调用结果：<br>
 *        + 0：成功；<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 */
- (int)setLocalStreamPriority:(ByteRTCStreamIndex)streamIndex priority:(ByteRTCStreamPriority)priority;
/**
 * 
 * @type api
 * @brief 设置 RTC SDK 内部采集时的视频采集参数。<br>
 *        如果你的项目使用了 SDK 内部采集模块，可以通过本接口指定视频采集参数，包括模式、分辨率、帧率。
 * @param streamIndex 源对应的流的索引。参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param captureConfig 视频采集参数。参看: ByteRTCVideoCaptureConfig{@link #ByteRTCVideoCaptureConfig}。
 * @return 方法调用结果：<br>
 *        + 0：成功；<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 * @note  
 * + 本接口在引擎创建后可调用，调用后立即生效。建议在调用 startVideoCapture:deviceId:{@link #ByteRTCEngineEx#startVideoCapture:deviceId:} 前调用本接口。<br>
 * + 如果调用本接口前使用内部模块开始视频采集，采集参数默认为 Auto 模式。
 */
- (int)setVideoCaptureConfig:(ByteRTCStreamIndex)streamIndex
               captureConfig:(ByteRTCVideoCaptureConfig* _Nullable)captureConfig;

#if BYTERTC_TARGET_MAC
/**
 * 
 * @type api
 * @brief 立即开启内部视频采集。默认为关闭状态。  <br>
 *        内部视频采集指：使用 RTC SDK 内置视频采集模块，进行采集。<br>
 *        调用该方法后，本地用户会收到 rtcEngine:onVideoDeviceStateChanged:device_type:device_state:device_error:{@link #ByteRTCEngineDelegate#rtcEngine:onVideoDeviceStateChanged:device_type:device_state:device_error:} 的回调。  <br>
 *        本地用户在非隐身状态下调用该方法后，房间中的其他用户会收到 rtcEngine:onUserStartVideoCapture:info:{@link #ByteRTCEngineDelegate#rtcEngine:onUserStartVideoCapture:info:} 的回调。
 * @param streamIndex 源对应的流的索引。参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param deviceId 设备 ID
 * @return 方法调用结果：<br>
 *        + 0：成功；<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 * @note  
 *       + 调用 stopVideoCapture:{@link #ByteRTCEngineEx#stopVideoCapture:} 可以停止内部视频采集。否则，只有当销毁引擎实例时，内部视频采集才会停止。  <br>
 *       + 创建引擎后，无论是否发布视频数据，你都可以调用该方法开启内部视频采集。只有当（内部或外部）视频采集开始以后视频流才会发布。  <br>
 *       + 如果需要从自定义视频采集切换为内部视频采集，你必须先关闭自定义采集，再调用此方法手动开启内部采集。
 */
- (int)startVideoCapture:(ByteRTCStreamIndex)streamIndex deviceId:(NSString* _Nonnull)deviceId;
#endif
/**
 * 
 * @type api
 * @brief 立即关闭内部视频采集。默认为关闭状态。  <br>
 *        内部视频采集指：使用 RTC SDK 内置视频采集模块，进行采集。<br>
 *        调用该方法后，本地用户会收到 rtcEngine:onVideoDeviceStateChanged:device_type:device_state:device_error:{@link #ByteRTCEngineDelegate#rtcEngine:onVideoDeviceStateChanged:device_type:device_state:device_error:} 的回调。  <br>
 *        非隐身用户进房后调用该方法，房间中的其他用户会收到 rtcEngine:onUserStopVideoCapture:info:{@link #ByteRTCEngineDelegate#rtcEngine:onUserStopVideoCapture:info:} 的回调。
 * @param streamIndex 源对应的流的索引。参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @return 方法调用结果：<br>
 *        + 0：成功；<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 * @note  
 *       + 调用 startVideoCapture:deviceId{@link #ByteRTCEngineEx#startVideoCapture:deviceId:} 可以开启内部视频采集。  <br>
 *       + 如果不调用本方法停止内部视频采集，则只有当销毁引擎实例时，内部视频采集才会停止。
 */
- (int)stopVideoCapture:(ByteRTCStreamIndex)streamIndex;

/**
 * 
 * @type api
 * @brief 设置音频源的整体音量。
 * @param streamIndex 源对应的流的索引。参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param volume 音量值和原始音量的比值，范围是 [0, 400]，单位为 %，自带溢出保护。<br>
 *               只改变音频数据的音量信息，不涉及本端硬件的音量调节。<br>
 *               为保证更好的通话质量，建议将 volume 值设为 [0,100]。
 * @return  
 *        + 0: 调用成功。<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 */
- (int)setAudioSourceVolume:(ByteRTCStreamIndex)streamIndex volume:(int)volume;
/**
 * 
 * @type api
 * @brief 设置音频源的内容。单个音频源的内容可以包含一个或多个原始音频来源。
 * @param streamIndex 源对应的流的索引。参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param config 内容构成。参看 ByteRTCAudioContentTypeConfig{@link #ByteRTCAudioContentTypeConfig}。
 * @return  
 *        + 0: 调用成功。<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 */
- (int)setAudioContentType:(ByteRTCStreamIndex)streamIndex config:(ByteRTCAudioContentTypeConfig* _Nonnull)config;
/**
 * 
 * @type api
 * @brief 设置音频流的编码配置。
 * @param streamIndex 源对应的流的索引。参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param config 编码设置。参看 ByteRTCAudioEncodeConfig{@link #ByteRTCAudioEncodeConfig}。
 * @return  
 *        + 0: 调用成功。<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 */
- (int)setAudioEncodeConfig:(ByteRTCStreamIndex)streamIndex config:(ByteRTCAudioEncodeConfig* _Nonnull)config;

/**
* 
* @type api
* @brief 设置多路流编码配置，在开启simulcast功能下可同时设置4路分辨率的流
* @param stream_index 视频频流的索引。参看 StreamIndex{@link #StreamIndex}。
* @param solutions 编码设置。参看 ByteRTCVideoEncoderConfig{@link #ByteRTCVideoEncoderConfig}。
* @param solution_num 需要传入的solutions数组大小。
* @return  
*        + 0: 调用成功。<br>
*        + < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
*/
- (int)setVideoEncoderConfig:(ByteRTCStreamIndex)streamIndex config:(NSArray <ByteRTCVideoEncoderConfig *> * _Nullable)videoSolutions;

@end

