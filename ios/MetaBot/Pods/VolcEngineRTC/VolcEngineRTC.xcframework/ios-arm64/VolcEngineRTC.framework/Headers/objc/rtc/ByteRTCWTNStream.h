/*
 * Copyright (c) 2021 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief ByteRTCWTNStream
*/

#import <Foundation/Foundation.h>
#import "ByteRTCVideoDefines.h"
/**
 * 
 * @type keytype
 * @brief WTN 媒体流状态
 */

typedef NS_ENUM(NSInteger, ByteRTCWTNSubscribeState) {
    /**
     * 
     * @brief 订阅 WTN 媒体流
     */
    
    ByteRTCWTNSubscribeStateSubscribe = 0,
    /**
     * 
     * @brief 取消订阅 WTN 媒体流
     */
    
    ByteRTCWTNSubscribeStateUnsubscribe = 1
};
/**
 * 
 * @type keytype
 * @brief WTN 媒体流订阅状态改变的原因
 */

typedef NS_ENUM(NSInteger, ByteRTCWTNSubscribeStateChangeReason) {
    /**
     * 
     * @brief 订阅 WTN 媒体流成功
     */
    
    ByteRTCWTNSubscribeStateChangeReasonSubscribe = 0, 
    /**
     * 
     * @brief 其他原因订阅失败
     */
    
    ByteRTCWTNSubscribeStateChangeReasonUnsubscribe = 1300,
    /**
     * 
     * @brief 订阅失败，拉流时远端未发布
     */
    
    ByteRTCWTNSubscribeStateChangeReasonRemotePublish = 1301,   
    /**
     * 
     * @brief 订阅失败，超出单端订阅上限。一个引擎实例最多拉 5 路流。
     */
    
    ByteRTCWTNSubscribeStateChangeReasonOverClientSubscribeStreamLimit = 1310,  
    /**
     * 
     * @brief 订阅失败。超出单流订阅人数上限。该限制由 RTC 服务端决定。
     */
    
    ByteRTCWTNSubscribeStateChangeReasonOverStreamSubscribeUserLimit = 1311,   
    /**
     * 
     * @brief 订阅失败。超出单流订阅请求 QPS 上限。该限制由 RTC 服务端决定。
     */
    
    ByteRTCWTNSubscribeStateChangeReasonOverStreamSubscribeRequestLimit = 1312    
};


/**
 * 
 * @type callback
 * @brief WTN 事件回调接口。 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 */

BYTERTC_APPLE_EXPORT @protocol ByteRTCWTNStreamDelegate<NSObject>
/**
* 
* @type callback
* @valid since 3.60.
* @brief 通话中本地设备接收订阅的远端 WTN 视频流的流 ID 以及远端 WTN 视频流统计信息。
* @param streamId WTN 流 ID
* @param videoStats 远端 WTN 视频流的统计信息，详见 ByteRTCRemoteVideoStats{@link #ByteRTCRemoteVideoStats}。
* @list WTN
* @order 0
*/

- (void)onWTNRemoteVideoStats:(NSString* _Nonnull)streamId
       videoStats:(const ByteRTCRemoteVideoStats* _Nonnull)videoStats;
/**
* 
* @type callback
* @valid since 3.60.
* @brief 通话中本地设备接收订阅的远端 WTN 音频流的流 ID 以及远端 WTN 音频流统计信息。
* @param streamId WTN 流 ID
* @param audioStats 远端 WTN 音频流的统计信息，详见 ByteRTCRemoteAudioStats{@link #ByteRTCRemoteAudioStats}。
* @list WTN
* @order 1
*/

- (void)onWTNRemoteAudioStats:(NSString* _Nonnull)streamId
       audioStats:(const ByteRTCRemoteAudioStats* _Nonnull)audioStats;

/**
* 
* @type callback
* @valid since 3.60. 自 3.60 起，该回调替代了 `rtcEngine:onPlayPublicStreamResult:errorCode:` 方法中的 WTN 视频流订阅状态变化通知功能。如果你已升级至 3.60 及以上版本，并且仍在使用该方法，请迁移到此回调。
* @brief WTN 视频流订阅状态变化回调 <br>
*        通过 subscribeWTNVideoStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNVideoStream:subscribe:} 订阅 WTN 视频流后，可以通过本回调获取订阅结果。
* @param streamId WTN 视频流的 ID
* @param state 视频流状态码，参看 ByteRTCWTNSubscribeState{@link #ByteRTCWTNSubscribeState}。
* @param reason 订阅状态发生变化的原因，参看 ByteRTCWTNSubscribeStateChangeReason{@link #ByteRTCWTNSubscribeStateChangeReason}。
* @list WTN
* @order 2
*/


- (void)onWTNVideoSubscribeStateChanged:(NSString *_Nonnull)streamId
        state:(ByteRTCWTNSubscribeState)state reason:(ByteRTCWTNSubscribeStateChangeReason)reason;
/**
* 
* @type callback
* @valid since 3.60. 自 3.60 起，该回调替代了 `rtcEngine:onPlayPublicStreamResult:errorCode:` 方法中的 WTN 音频流订阅状态变化通知功能。如果你已升级至 3.60 及以上版本，并且仍在使用该方法，请迁移到此回调。
* @brief WTN 音频流订阅状态变化回调 <br>
*        通过 subscribeWTNAudioStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNAudioStream:subscribe:} 订阅 WTN 音频流后，可以通过本回调获取订阅结果。
* @param streamId WTN 音频流的 ID
* @param state 音频流状态码，参看 ByteRTCWTNSubscribeState{@link #ByteRTCWTNSubscribeState}。
* @param reason 订阅状态发生变化的原因，参看 ByteRTCWTNSubscribeStateChangeReason{@link #ByteRTCWTNSubscribeStateChangeReason}。
* @list WTN
* @order 2
*/

- (void)onWTNAudioSubscribeStateChanged:(NSString *_Nonnull)streamId
        state:(ByteRTCWTNSubscribeState)state reason:(ByteRTCWTNSubscribeStateChangeReason)reason;

/**
 * 
 * @type callback
 * @valid since 3.60. 自 3.60 起，该回调替代了 `onWTNFirstRemoteVideoFrameDecoded:withFrameInfo:`。如果你已升级至 3.60 及以上版本，并且仍在使用该方法，请迁移到此回调。
 * @brief WTN 流的首帧视频解码成功 <br>
 *        关于 订阅 WTN 流，详见 subscribeWTNVideoStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNVideoStream:subscribe:}。
 * @param streamId WTN 流 ID
 * @param frameInfo 视频帧信息，参看 ByteRTCVideoFrameInfo{@link #ByteRTCVideoFrameInfo}
 * @list WTN
 * @order 4
 */

- (void)onWTNFirstRemoteVideoFrameDecoded:(NSString * _Nonnull)streamId withFrameInfo:(ByteRTCVideoFrameInfo * _Nonnull)frameInfo NS_SWIFT_NAME(onWTNFirstRemoteVideoFrameDecoded(streamId:withFrameInfo:));

/**
* 
* @type callback
* @valid since 3.60. 自 3.60 起，该回调替代了 `rtcEngine:onFirstPublicStreamAudioFrame:`。如果你已升级至 3.60 及以上版本，并且仍在使用该方法，请迁移到此回调。
* @brief WTN 流的首帧音频解码成功 <br>
*        关于订阅 WTN 音频流，详见 subscribeWTNAudioStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNAudioStream:subscribe:}。
* @param streamId WTN 流 ID
* @list WTN
* @order 3
*/

- (void)onWTNFirstRemoteAudioFrame:(NSString * _Nonnull)streamId;

/**
 * 
 * @type callback
 * @valid since 3.60. 自 3.60 起，该回调替换了 `rtcEngine:onPublicStreamSEIMessageReceived:andMessage:andSourceType:` 来实现下述功能。如果你已升级至 3.60 及以上版本，并且仍在使用该方法，请迁移到此回调。
 * @brief 回调 WTN 流中包含的 SEI 信息。 <br>
 *        subscribeWTNAudioStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNAudioStream:subscribe:}/subscribeWTNVideoStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNVideoStream:subscribe:} 接口启动拉 WTN 音频/视频流功能后，通过此回调收到 WTN 流中的 SEI 消息。
 * @param streamId WTN 流 ID。
 * @param channelId SEI 消息通道 ID，取值范围 [0 - 255]。通过此参数，你可以为不同接受方设置不同的 ChannelID，这样不同接收方可以根据回调中的 ChannelID 选择应关注的 SEI 信息。
 * @param message 收到的 SEI 消息内容。 <br>
 *                通过调用客户端 `sendPublicStreamSEIMessage` 插入的 SEI 信息。
 *                当 WTN 流中的多路视频流均包含有 SEI 信息：SEI 不互相冲突时，将通过多次回调分别发送；SEI 在同一帧有冲突时，则只有一条流中的 SEI 信息被透传并融合到 WTN 流中。
 * @note 通过 Open API 插入的自定义信息，应通过回调 onWTNDataMessageReceived:andMessage:andSourceType:{@link #ByteRTCWTNStreamDelegate#onWTNDataMessageReceived:andMessage:andSourceType:} 获取。
 * @list WTN
 * @order 5
 */

- (void)onWTNSEIMessageReceived:(NSString* _Nonnull)streamId andChannelId:(int)channelId andMessage:(NSData* _Nonnull)message;

/**           
 * 
 * @type callback
 * @valid since 3.60.
 * @brief 回调 WTN 流中包含的数据信息。 <br>
 *        通过 subscribeWTNAudioStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNAudioStream:subscribe:}/subscribeWTNVideoStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNVideoStream:subscribe:} 订阅 WTN 流后，可以通过本回调获取 WTN 流中的数据消息，包括调用 Open API 发送的 SEI 消息和音量回调。
 * @param streamId WTN 流 ID
 * @param message 收到的数据消息内容，如下： <br>
 * - 调用 WTN 流 OpenAPI 发送的自定义消息。
 * - 媒体流音量变化，需要通过 WTN 流 OpenAPI 开启回调。JSON 格式说明如下：
 * JSON 格式说明如下：<br/>
 * {<br/>
 * "Type" : "VolumeIndication", 
 * "VolumeInfos" : [ 
 * {<br/>
 * "RoomId":"1000001", 
 * "UserId":"1000001", 
 * "StreamType":0, 
 * "LinearVolume":1 
 * }<br/>
 * @param sourceType 数据消息来源，参看 ByteRTCDataMessageSourceType{@link #ByteRTCDataMessageSourceType}。
 * @note 通过调用客户端 API 插入的 SEI 信息，应通过回调 onWTNSEIMessageReceived:andChannelId:andMessage:{@link #ByteRTCWTNStreamDelegate#onWTNSEIMessageReceived:andChannelId:andMessage:} 获取。
 * @list WTN
 */

- (void)onWTNDataMessageReceived:(NSString* _Nonnull)streamId andMessage:(NSData* _Nonnull)message andSourceType:(ByteRTCDataMessageSourceType)sourceType;

@end

/**
 * 
 * @type api
 * @region WTN
 * @brief WTN 流处理
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCWTNStream :NSObject
/** 
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，此接口替代了 `startPlayPublicStream:` 和 `stopPlayPublicStream:` 方法来订阅/取消订阅指定 WTN 视频流，如果你使用了这两个方法，请迁移至此接口。
 * @brief 订阅/取消订阅指定 WTN 视频流 <br>
 *        无论用户是否在房间内，都可以调用本接口订阅/取消订阅指定的 WTN 视频流。
 * @param streamId WTN 流 ID，如果指定流暂未发布，则本地客户端将在其开始发布后接收到流数据。
 * @param subscribe 是否订阅 WTN 流 <br>
 *       - true：订阅
  *       - false：取消订阅
 * @return
 *        - 0: 成功。同时将收到 
 onWTNAudioSubscribeStateChanged:state:reason:{@link #ByteRTCWTNStreamDelegate#onWTNAudioSubscribeStateChanged:state:reason:} 回调。
 *        - !0: 失败。当参数不合法或参数为空，调用失败。
 * @note
 *        - 一个客户端最多同时播放 5 路 WTN 流，请及时调用 subscribeWTNVideoStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNVideoStream:subscribe:}/subscribeWTNAudioStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNAudioStream:subscribe:} 取消订阅 WTN 流，避免订阅的 WTN 流数量超限。
 *        - 在调用本接口之前，建议先绑定渲染视图。
 *              - 调用 setWTNRemoteVideoCanvas:withCanvas:{@link #ByteRTCWTNStream#setWTNRemoteVideoCanvas:withCanvas:} 绑定内部渲染视图
 *              - 调用 setWTNRemoteVideoSink:withSink:withConfig:{@link #ByteRTCWTNStream#setWTNRemoteVideoSink:withSink:withConfig:} 绑定自定义渲染视图
 *        - 调用本接口后，可以通过 onWTNFirstRemoteVideoFrameDecoded:withFrameInfo:{@link #ByteRTCWTNStreamDelegate#onWTNFirstRemoteVideoFrameDecoded:withFrameInfo:} 回调 WTN 视频流的首帧解码情况
 *        - 调用本接口后，可以通过 onWTNSEIMessageReceived:andChannelId:andMessage:{@link #ByteRTCWTNStreamDelegate#onWTNSEIMessageReceived:andChannelId:andMessage:} 回调 WTN 流中包含的 SEI 信息。
 * @list WTN
 * @order 0
 */

- (int)subscribeWTNVideoStream:(NSString * _Nonnull)streamId subscribe:(bool)subscribe;

/** 
* 
* @type api
* @valid since 3.60. 自 3.60 起，此接口替代了 `startPlayPublicStream:` 和 `stopPlayPublicStream:` 方法来订阅/取消订阅指定 WTN 音频流，如果你使用了这两个方法，请迁移至此接口。
* @brief 订阅/取消订阅指定 WTN 音频流 <br>
*        无论用户是否在房间内，都可以调用本接口订阅/取消订阅指定的 WTN 音频流。
* @param streamId WTN 流 ID，如果指定流暂未发布，则本地客户端将在其开始发布后接收到流数据。
* @param subscribe 是否订阅 WTN 流 <br>
*       - true：订阅
*       - false：取消订阅
* @return
*        - 0: 成功。同时将收到 onWTNVideoSubscribeStateChanged:state:reason:{@link #ByteRTCWTNStreamDelegate#onWTNVideoSubscribeStateChanged:state:reason:} 回调。
*        - !0: 失败。当参数不合法或参数为空，调用失败。
* @note
*        - 一个客户端最多同时播放 5 路 WTN 流，请及时调用 subscribeWTNVideoStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNVideoStream:subscribe:}/subscribeWTNAudioStream:subscribe:{@link #ByteRTCWTNStream#subscribeWTNAudioStream:subscribe:} 取消订阅 WTN 流，避免订阅的 WTN 流数量超限。
*        - 在调用本接口之前，建议先绑定渲染视图。
*              - 调用 setWTNRemoteVideoCanvas:withCanvas:{@link #ByteRTCWTNStream#setWTNRemoteVideoCanvas:withCanvas:}  绑定内部渲染视图
*              - 调用 setWTNRemoteVideoSink:withSink:withConfig:{@link #ByteRTCWTNStream#setWTNRemoteVideoSink:withSink:withConfig:} 绑定自定义渲染视图
*        - 调用本接口后，可以通过 onWTNFirstRemoteAudioFrame:{@link #ByteRTCWTNStreamDelegate#onWTNFirstRemoteAudioFrame:} 回调 WTN 音频流首帧解码情况。
*        - 调用本接口后，可以通过 onWTNSEIMessageReceived:andChannelId:andMessage:{@link #ByteRTCWTNStreamDelegate#onWTNSEIMessageReceived:andChannelId:andMessage:} 回调 WTN 流中包含的 SEI 信息。
* @list WTN
* @order 1
*/

- (int)subscribeWTNAudioStream:(NSString * _Nonnull)streamId subscribe:(bool)subscribe;

/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `setPublicStreamVideoCanvas:withCanvas:` 方法来实现下述功能。你已升级至 3.60 及以上版本，并且仍在使用该方法，请迁移到此接口。
 * @brief 为指定 WTN 流绑定内部渲染视图
 * @param streamId WTN 流 ID
 * @param canvas 内部渲染视图，如果需要解除视频的绑定视图，把 videoCanvas 设置为空。详见 ByteRTCVideoCanvas{@link #ByteRTCVideoCanvas}
 * @return
 *        - 0：成功
 *        - !0：失败
 * @list WTN
 * @order 2
 */

- (int)setWTNRemoteVideoCanvas:(NSString *_Nonnull)streamId
             withCanvas:(ByteRTCVideoCanvas * _Nullable)canvas NS_SWIFT_NAME(setWTNRemoteVideoCanvas(_:withCanvas:));

/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `setPublicStreamVideoSink:withSink:withPixelFormat:` 方法来实现下述功能。你已升级至 3.60 及以上版本，并且仍在使用该方法，请迁移到此接口。
 * @brief 为指定 WTN 流绑定自定义渲染器。详见[自定义视频渲染](https:
 * @param streamId WTN 流 ID
 * @param videoSink 自定义视频渲染器，需要释放渲染器资源时，将 videoSink 设置为 `null`。参看 ByteRTCVideoSinkDelegate{@link #ByteRTCVideoSinkDelegate}
 * @param config 远端视频帧回调配置，参看 ByteRTCRemoteVideoSinkConfig{@link #ByteRTCRemoteVideoSinkConfig}
 * @return
 *        - 0: 成功
 *        - <0: 失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 * @list WTN
 * @order 3
 */

- (int)setWTNRemoteVideoSink:(NSString *_Nonnull)streamId
                 withSink:(id<ByteRTCVideoSinkDelegate> _Nullable)videoSink
          withConfig:(ByteRTCRemoteVideoSinkConfig*)config   NS_SWIFT_NAME(setWTNRemoteVideoSink(_:withSink:withConfig:));

/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `setPublicStreamAudioPlaybackVolume:volume:` 方法来实现下述功能。你已升级至 3.60 及以上版本，并且仍在使用该方法，请迁移到此接口。
 * @brief 调节 WTN 流的音频播放音量。
 * @param streamId WTN 流 ID
 * @param volume 音频播放音量值和原始音量值的比值，该比值的范围是 `[0, 400]`，单位为 %，且自带溢出保护。为保证更好的音频质量，建议设定在 `[0, 100]` 之间，其中 100 为系统默认值。
 * @return
 *         - 0: 成功调用。
 *         - -2: 参数错误。
 * @list WTN
 * @order 4
 */

- (int)setWTNRemoteAudioPlaybackVolume:(NSString *_Nonnull)streamId volume:(NSInteger)volume;
/**
* 
* @type api
* @valid since 3.60.
* @brief 设置 WTN 流回调接口
* @param delegate WTN 回调类，参看 ByteRTCWTNStreamDelegate{@link #ByteRTCWTNStreamDelegate}。
* @list WTN
* @order 5
*/

- (void)setWTNStreamDelegate:(_Nullable id<ByteRTCWTNStreamDelegate>)delegate;
@end
