/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room
*/

#import <CoreMedia/CMTime.h>
#import "ByteRTCRTSRoom.h"
#import "rtc/ByteRTCRangeAudio.h"
#import "rtc/ByteRTCSpatialAudio.h"

@class ByteRTCRoom;
/**
 * 
 * @type callback
 * @brief 房间事件回调接口 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 
 */

@protocol ByteRTCRoomDelegate <NSObject>
@optional
 /**
  * 
  * @hidden
  * @type callback
  * @brief 房间状态改变回调，加入房间、异常退出房间、发生房间相关的警告或错误时会收到此回调。
  * @param rtcRoom ByteRTCAudioRoom 实例
  * @param roomId 房间 ID。
  * @param uid 用户 ID。
  * @param state 房间状态码。 <br>
  *              - 0: 加入房间成功。
  *              - 1: 加入房间失败、异常退房、发生房间相关的警告或错误。
  *              - 2: 离开房间。
  * @param reason 房间状态发生变化的原因。参看 ByteRTCRoomStateChangeReason{@link #ByteRTCRoomStateChangeReason}。
  * @list 房间管理
  */
 
- (void)rtcRoom:(ByteRTCRoom *_Nonnull)rtcRoom onRoomStateChangedWithReason:(NSString *_Nonnull)roomId
             withUid:(nonnull NSString *)uid
           state:(ByteRTCRoomState)state
           reason:(ByteRTCRoomStateChangeReason)reason;

 /**
  * 
  * @type callback
  * @region 多房间
  * @brief 房间状态改变回调，加入房间、异常退出房间、发生房间相关的警告或错误时会收到此回调。
  * @param rtcRoom ByteRTCAudioRoom 实例
  * @param roomId 房间 ID。
  * @param uid 用户 ID。
  * @param state 房间状态码。 <br>
  *              - 0: 加入房间成功。
  *              - !0: 加入房间失败、异常退房、发生房间相关的警告或错误。具体原因参看 ByteRTCErrorCode{@link #ByteRTCErrorCode} 及 ByteRTCWarningCode{@link #ByteRTCWarningCode}。
  * @param extraInfo 额外信息，如 `{"elapsed":1187,"join_type":0}`。 <br>
  *                  `join_type`表示加入房间的类型，`0`为首次进房，`1`为重连进房。 <br>
  *                  `elapsed`表示加入房间耗时，即本地用户从调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 到加入房间成功所经历的时间间隔，单位为 ms。
  * @list 房间管理
  * @order 0
  */
 
- (void)rtcRoom:(ByteRTCRoom *_Nonnull)rtcRoom onRoomStateChanged:(NSString *_Nonnull)roomId
             withUid:(nonnull NSString *)uid
           state:(NSInteger)state
           extraInfo:(NSString *_Nonnull)extraInfo;

/**
 * 
 * @deprecated since 3.60, refer to [Upgrade Guide](https:
 * @type callback
 * @brief 流状态改变回调，发生流相关的警告或错误时会收到此回调。
 * @param rtcRoom `ByteRTCRoom` 实例
 * @param roomId 房间 ID。
 * @param uid 用户 ID。
 * @param state 流状态码，参看 ByteRTCErrorCode{@link #ByteRTCErrorCode} 及 ByteRTCWarningCode{@link #ByteRTCWarningCode}。
 * @param extraInfo 附加信息，目前为空。
 * @list 音视频传输
 */

- (void)rtcRoom:(ByteRTCRoom *_Nonnull)rtcRoom onStreamStateChanged:(NSString *_Nonnull)roomId
            withUid:(nonnull NSString *)uid
          state:(NSInteger)state
          extraInfo:(NSString *_Nonnull)extraInfo;
/**
 * 
 * @type callback
 * @region 多房间
 * @brief 离开房间成功回调。 <br>
 *        用户调用 leaveRoom{@link #ByteRTCRoom#leaveRoom} 方法后，SDK 会停止所有的发布订阅流，并释放所有通话相关的音视频资源。SDK 完成所有的资源释放后通过此回调通知用户。
 * @param rtcRoom  ByteRTCRoom 对象。
 * @param stats 保留参数，目前为空。
 * @note
 *       - 用户调用 leaveRoom{@link #ByteRTCRoom#leaveRoom} 方法离开房间后，如果立即调用 destroy{@link #ByteRTCRoom#destroy} 销毁房间实例或 destroyRTCEngine{@link #ByteRTCEngine#destroyRTCEngine} 方法销毁 RTC 引擎，则将无法收到此回调事件。
 *       - 离开房间结束通话后，如果 App 需要使用系统音视频设备，则建议在收到此回调后再初始化音视频设备，否则可能由于 SDK 占用了导致 App 初始化音视频设备失败。
 * @list 房间管理
 * @order 2
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onLeaveRoom:(ByteRTCRoomStats *_Nonnull)stats;

 /**
  * 
  * @hidden
  * @deprecated since 3.41 and will be deleted in 3.51, use rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} and rtcRoom:onStreamStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onStreamStateChanged:withUid:state:extraInfo:} instead.
  * @type callback
  * @brief 发生警告回调。 <br>
  *        SDK 运行时出现了警告。SDK 通常会自动恢复，警告信息可以忽略。
  * @param rtcRoom ByteRTCRoom 对象。
  * @param warningCode 警告码，详见枚举类型 ByteRTCWarningCode{@link #ByteRTCWarningCode} 。
  * @list 房间管理
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRoomWarning:(ByteRTCWarningCode)warningCode __deprecated_msg("Will be removed in new version");
/**
 * 
 * @type callback
 * @brief 发布端调用 setMultiDeviceAVSync:{@link #ByteRTCRoom#setMultiDeviceAVSync:} 后音视频同步状态发生改变时，会收到此回调。
 * @param rtcRoom ByteRTCRoom 实例。
 * @param state 音视频同步状态，参看 ByteRTCAVSyncState{@link #ByteRTCAVSyncState}。
 * @list 音视频传输
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onAVSyncStateChange:(ByteRTCAVSyncState)state;
/**
 * 
 * @type callback
 * @valid since 3.60. 自 3.60 起，该回调替代了 `rtcRoom:onStreamStateChanged:withUid:state:extraInfo:` 方法中的本地视频发布状态变更通知功能。如果你已升级至 3.60 及以上版本 SDK，且还在使用该方法，请迁移至该回调。
 * @brief 本地视频流发布状态改变回调。
 * @param rtcRoom ByteRTCRoom 实例。
 * @param streamId 流 ID。
 * @param info 流信息。
 * @param state 发布状态码，参看 ByteRTCPublishState{@link #ByteRTCPublishState}。
 * @param reason 本地视频流发布状态改变的具体原因，参看 ByteRTCPublishStateChangeReason{@link #ByteRTCPublishStateChangeReason}。
 * @list 音视频传输
 * @order 0
 */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onVideoPublishStateChanged:(NSString *_Nonnull)streamId
           info:(ByteRTCStreamInfo* _Nonnull)info
           state:(ByteRTCPublishState)state
           reason:(ByteRTCPublishStateChangeReason)reason;

/**
 * 
 * @type callback
 * @valid since 3.60. 自 3.60 起，该回调替代了 `rtcRoom:onStreamStateChanged:withUid:state:extraInfo:` 方法中的本地音频流发布状态变更通知功能。如果你已升级至 3.60 及以上版本 SDK，且还在使用该方法，请迁移至该回调。
 * @brief 本地音频流发布状态改变回调。
 * @param rtcRoom ByteRTCRoom 实例。
 * @param streamId 流 ID。
 * @param info 流信息。
 * @param state 发布状态码，参看 ByteRTCPublishState{@link #ByteRTCPublishState}。
 * @param reason 本地音频流发布状态改变的具体原因，参看 ByteRTCPublishStateChangeReason{@link #ByteRTCPublishStateChangeReason}。
 * @list 音视频传输
 * @order 0
 */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onAudioPublishStateChanged:(NSString *_Nonnull)streamId
           info:(ByteRTCStreamInfo* _Nonnull)info
           state:(ByteRTCPublishState)state
           reason:(ByteRTCPublishStateChangeReason)reason;

/**
 * 
 * @type callback
 * @valid since 3.60. 自 3.60 起，该回调替代了 `rtcRoom:onStreamStateChanged:withUid:state:extraInfo:` 和 `rtcRoom:onStreamSubscribed:userId:subscribeConfig:` 方法中的视频流订阅状态发生变更通知功能。如果你已升级至 3.60 及以上版本 SDK，且还在使用这两个方法，请迁移至该回调。
 * @brief 视频流订阅状态发生改变回调。
 * @param rtcRoom ByteRTCRoom 实例。
 * @param streamId 流 ID。
 * @param info 流信息。
 * @param state 订阅状态码，参看 ByteRTCSubscribeState{@link #ByteRTCSubscribeState}。
 * @param reason 视频订阅状态改变的具体原因，参看 ByteRTCSubscribeStateChangeReason{@link #ByteRTCSubscribeStateChangeReason}。
 * @note 当调用 subscribeStreamVideo:subscribe:{@link #ByteRTCRoom#subscribeStreamVideo:subscribe:} 后会收到该通知。
 * @list 音视频传输
 * @order 0
 */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onVideoSubscribeStateChanged:(NSString *_Nonnull)streamId
           info:(ByteRTCStreamInfo* _Nonnull)info
           state:(ByteRTCSubscribeState)state
           reason:(ByteRTCSubscribeStateChangeReason)reason;

/**
 * 
 * @type callback
 * @valid since 3.60. 自 3.60 起，该回调替代了 `rtcRoom:onStreamStateChanged:withUid:state:extraInfo:` 和 `rtcRoom:onStreamSubscribed:userId:subscribeConfig:` 方法中的音频流订阅状态发生变更通知功能。如果你已升级至 3.60 及以上版本 SDK，且还在使用这两个方法，请迁移至该回调。
 * @brief 音频流订阅状态发生改变回调。
 * @param rtcRoom ByteRTCRoom 实例。
 * @param streamId 流 ID。
 * @param info 流信息。
 * @param state 订阅状态码，参看 ByteRTCSubscribeState{@link #ByteRTCSubscribeState}。
 * @param reason 音频流订阅状态改变的具体原因，参看 ByteRTCSubscribeStateChangeReason{@link #ByteRTCSubscribeStateChangeReason}。
 * @note 当调用 subscribeStreamVideo:subscribe:{@link #ByteRTCRoom#subscribeStreamVideo:subscribe:} 后会收到该通知。
 * @list 音视频传输
 * @order 0
 */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onAudioSubscribeStateChanged:(NSString *_Nonnull)streamId
            info:(ByteRTCStreamInfo* _Nonnull)info
           state:(ByteRTCSubscribeState)state
           reason:(ByteRTCSubscribeStateChangeReason)reason;

/**
 * 
 * @type callback
 * @region 多房间
 * @brief 房间内通话统计信息回调。 <br>
 *        用户进房开始通话后，每 2s 收到一次本回调。
 * @param rtcRoom  ByteRTCRoom 对象。
 * @param stats 当前 ByteRTCRoom 统计数据，详见：ByteRTCRoomStats{@link #ByteRTCRoomStats}
 * @list 房间管理
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRoomStats:(ByteRTCRoomStats *_Nonnull)stats NS_SWIFT_NAME(rtcRoom(_:onRoomStats:));
/**
  * 
  * @hidden
  * @type callback
  * @region 多房间
  * @valid since 3.60.
  * @brief 房间事件回调。
  * @param rtcRoom  ByteRTCRoom 对象。
  * @param roomId 房间 ID。
  * @param uid 用户 ID。
  * @param state 房间事件状态。详见 ByteRTCRoomEvent{@link #ByteRTCRoomEvent}。
  * @param info 房间封禁时，包含封禁时间。详见 ByteRTCRoomEventInfo{@link #ByteRTCRoomEventInfo}。
  * @list 房间管理
  * @order 0
  */
  
- (void)rtcRoom:(ByteRTCRoom *_Nonnull)rtcRoom onRoomEvent:(NSString *_Nonnull)roomId uid:(NSString *_Nonnull)uid state:(ByteRTCRoomEvent)state info:(ByteRTCRoomEventInfo *_Nonnull)info;
/**
 * 
 * @type callback
 * @brief 本地流数据统计以及网络质量回调。 <br>
 *        本地用户发布流成功后，SDK 会周期性（2s）的通过此回调事件通知用户发布的流在此次统计周期内的质量统计信息。 <br>
 *        统计信息通过 ByteRTCLocalStreamStats{@link #ByteRTCLocalStreamStats} 类型的回调参数传递给用户，其中包括发送音视频比特率、发送帧率、编码帧率，网络质量等。
 * @param rtcRoom  ByteRTCRoom 对象。
 * @param streamId 流 ID。
 * @param info 流信息。
 * @param stats 当前房间本地流数据统计。详见：ByteRTCLocalStreamStats{@link #ByteRTCLocalStreamStats}
 * @list 音视频传输
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onLocalStreamStats:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info
stats:(ByteRTCLocalStreamStats *_Nonnull)stats NS_SWIFT_NAME(rtcRoom(_:onLocalStreamStats:));
/**
 * 
 * @type callback
 * @brief 本地订阅的远端音/视频流数据统计以及网络质量回调。 <br>
 *        本地用户订阅流成功后，SDK 会周期性（2s）的通过此回调事件通知用户订阅的流在此次统计周期内的质量统计信息，包括：发送音视频比特率、发送帧率、编码帧率，网络质量等。
 * @param rtcRoom  ByteRTCRoom 对象。
 * @param streamId 流 ID。
 * @param info 流信息。
 * @param stats 当前房间本地流数据统计。 详见：ByteRTCRemoteStreamStats{@link #ByteRTCRemoteStreamStats}
 * @list 音视频传输
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRemoteStreamStats:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info
stats:(ByteRTCRemoteStreamStats *_Nonnull)stats NS_SWIFT_NAME(rtcRoom(_:onRemoteStreamStats:));
/**
 * 
 * @type callback
 * @brief 远端可见用户加入房间，或房内不可见用户切换为可见的回调。 <br>
 *        1. 远端用户调用 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 方法将自身设为可见后加入房间时，房间内其他用户将收到该事件。 <br>
 *        2. 远端可见用户断网后重新连入房间时，房间内其他用户将收到该事件。 <br>
 *        3. 房间内隐身远端用户调用 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 方法切换至可见时，房间内其他用户将收到该事件。 <br>
 *        4. 新进房用户会收到进房前已在房内的可见用户的进房回调通知。
 * @param rtcRoom ByteRTCRoom 对象。
 * @param userInfo 用户信息，参看 ByteRTCUserInfo{@link #ByteRTCUserInfo}。
 * @list 房间管理
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onUserJoined:(ByteRTCUserInfo *_Nonnull)userInfo;
/**
 * 
 * @type callback
 * @brief 远端用户离开 RTC 房间，或切至不可见时，本地用户会收到此事件
 * @param rtcRoom `ByteRTCRoom` 实例
 * @param uid 离开房间，或切至不可见的的远端用户 ID。
 * @param reason 用户离开房间的原因： <br>
 *              - 0: 远端用户调用 leaveRoom{@link #ByteRTCRoom#leaveRoom} 主动退出房间。
 *              - 1: 远端用户因 Token 过期或网络原因等掉线。详细信息请参看[连接状态提示](https:
 *              - 2: 远端用户调用 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 切换至不可见状态。
 *              - 3: 服务端调用 OpenAPI 将远端用户踢出房间。
 * @list 房间管理
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onUserLeave:(NSString *_Nonnull)uid reason:(ByteRTCUserOfflineReason)reason;
/**
 * 
 * @type callback
 * @brief 当 SDK 检测到 Token 的进房权限将在 30 秒内过期时，触发该回调。
 *        收到该回调后，你需调用 updateToken:{@link #ByteRTCRTSRoom#updateToken:} 更新 Token 进房权限。
 * @param rtcRoom `ByteRTCRoom` 实例
 * @note 若 Token 进房权限过期且未及时更新： <br>
 *        - 用户此时尝试进房会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调，提示错误码为 `-1000` Token 无效；
 *        - 用户已在房间内则会被移出房间，本地用户会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调，提示错误码为 `-1009` Token 过期，同时远端用户会收到 rtcRoom:onUserLeave:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onUserLeave:reason:} 回调，提示原因为 `1` Token 进房权限过期。
 * @list 引擎管理
 */
 
- (void)onTokenWillExpire:( ByteRTCRoom *_Nonnull)rtcRoom;
/**
 * 
 * @type callback
 * @brief Token 发布权限过期前 30 秒将触发该回调。 <br>
 *        收到该回调后，你需调用 updateToken:{@link #ByteRTCRTSRoom#updateToken:} 更新 Token 发布权限。
 * @param rtcRoom `ByteRTCRoom` 实例
 * @note Token 发布权限过期后：
     *        - 已发布流或尝试发布流时，本端会收到 rtcRoom:onVideoPublishStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onVideoPublishStateChanged:info:state:reason:}、rtcRoom:onAudioPublishStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onAudioPublishStateChanged:info:state:reason:}回调，提示`kPublishStateChangeReasonNoPublishPermission`，没有发布权限。
     *        - 发布中的流将停止发布。远端用户会收到 rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}、rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:} 回调，提示该流已停止发布。
     * @list 引擎管理
 * @order 3
 */

- (void)onPublishPrivilegeTokenWillExpire:( ByteRTCRoom *_Nonnull)rtcRoom;
/**
 * 
 * @type callback
 * @brief Token 订阅权限过期前 30 秒将触发该回调。 <br>
 *        收到该回调后，你需调用 updateToken:{@link #ByteRTCRTSRoom#updateToken:} 更新 Token 订阅权限有效期。
 * @param rtcRoom `ByteRTCRoom` 实例
 * @note 若收到该回调后未及时更新 Token，Token 订阅权限过期后，尝试新订阅流会失败，已订阅的流会取消订阅，并且会收到 rtcRoom:onVideoSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onVideoSubscribeStateChanged:info:state:reason:}、rtcRoom:onAudioSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onAudioSubscribeStateChanged:info:state:reason:} 回调，提示错误码为 `-1003` 没有订阅权限。
 * @list 引擎管理
 */

- (void)onSubscribePrivilegeTokenWillExpire:( ByteRTCRoom *_Nonnull)rtcRoom;
 /**
  * 
  * @deprecated since 3.36 and will be deleted in 3.51, use rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}instead.
  * @type callback
  * @brief 以下情况会触发此回调： <br>
  *        - 房间内的用户发布新的音视频流时，房间内的其他用户会收到此回调通知。
  *        - 房间内的用户原音视频流被移出后，又重新发布音视频流时，房间内的其他用户会收到此回调通知。
  *        - 用户刚加入房间时，会收到此回调，包含房间中所有已发布的流。
  * @param rtcRoom  ByteRTCRoom 对象。
  * @param stream 流属性，参看 ByteRTCStream{@link #ByteRTCStream} 。
  * @list 房间管理
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onStreamAdd:(id<ByteRTCStream> _Nonnull)stream __deprecated_msg("Will be removed in new version");
 /**
  * 
  * @hidden
  * @deprecated since 3.36 and will be deleted in 3.51, use rtcRoom:onUserUnpublishStream:type:reason: instead.
  * @type callback
  * @brief 房间内的远端用户停止发布音视频流时，本地用户会收到此回调通知。
  * @param rtcRoom ByteRTCRoom 对象
  * @param uid 远端流来源的用户 ID 。
  * @param stream 流的属性，参看 ByteRTCStream{@link #ByteRTCStream}。
  * @param reason 远端流移除的原因，参看 ByteRTCStreamRemoveReason{@link #ByteRTCStreamRemoveReason} 。
  * @list 房间管理
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onStreamRemove:(NSString *_Nonnull)uid
              stream:(id<ByteRTCStream> _Nonnull)stream
              reason:(ByteRTCStreamRemoveReason)reason
              __deprecated_msg("Will be removed in new version");

/**
 * 
 * @hidden for internal use only
 * @type callback
 * @region 多房间
 * @brief 当发布流成功的时候回调该事件
 * @param rtcRoom  ByteRTCRoom 对象。
 * @param userId 用户 ID
 * @param isScreen 该流是否是屏幕流 <br>
 *
 */


- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onStreamPublishSuccess:(NSString *_Nonnull)userId isScreen:(BOOL)isScreen;

/**
  * 
  * @type callback
  * @valid since 3.60.
  * @brief 发布端调用 setMultiDeviceAVSync:{@link #ByteRTCRoom#setMultiDeviceAVSync:} 后音视频同步状态发生错误时，会收到此回调。
  * @param rtcRoom ByteRTCRoom 对象
  * @param roomId 房间 ID。
  * @param userId 用户 ID。
  * @param eventCode 音视频同步状态错误，参看 ByteRTCAVSyncEvent{@link #ByteRTCAVSyncEvent}。
  * @list 音视频传输
  * @order 1
  */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onAVSyncEvent:(NSString *_Nonnull)roomId userId:(NSString *_Nonnull)userId eventCode:(ByteRTCAVSyncEvent)eventCode;

/**
 * 
 * @type callback
 * @valid since 3.60. 自版本 3.60 起，该回调替换了 `onUserPublishStream`、`onUserUnpublishStream`、`onUserPublishScreen` 和 `onUserUnpublishScreen` 方法。如果您已升级到 SDK 版本 3.60 或以上，且仍在使用这些方法，请迁移到该回调。
 * @region 房间管理
 * @brief 房间内远端用户发布或取消发布视频流的回调。
 * @param rtcRoom ByteRTCRoom 对象。
 * @param streamId 流 ID。
 * @param info 流信息。
 * @param isPublish 流是否发布。
 *         + `true`：已发布。
 *         + `false`：已取消发布。
 * @note 当房间内的远端用户调用 publishStreamVideo:{@link #ByteRTCRoom#publishStreamVideo:} 成功发布采集的视频流时，本地用户会收到该回调，此时本地用户可以自行选择是否调用 subscribeStreamVideo:subscribe:{@link #ByteRTCRoom#subscribeStreamVideo:subscribe:} 订阅此流。
 * @list 音视频传输
 * @order 2
 */

- (void)rtcRoom:(ByteRTCRoom* _Nonnull)rtcRoom onUserPublishStreamVideo:(NSString* _Nonnull)streamId info:(ByteRTCStreamInfo* _Nonnull)info isPublish:(BOOL)isPublish;

/**
 * 
 * @type callback
 * @valid since 3.60. 自版本 3.60 起，该回调替换了 `onUserPublishStream`、`onUserUnpublishStream`、`onUserPublishScreen` 和 `onUserUnpublishScreen` 方法。如果您已升级到 SDK 版本 3.60 或以上，且仍在使用这些方法，请迁移到该回调。
 * @region 房间管理
 * @brief 房间内远端用户发布或取消发布音频流的回调。
 * @param rtcRoom ByteRTCRoom 对象。
 * @param streamId 流 ID。
 * @param info 流信息。
 * @param isPublish 流是否发布。
 *         + `true`：已发布。
 *         + `false`：已取消发布。
 * @note 当房间内的远端用户调用 publishStreamAudio:{@link #ByteRTCRoom#publishStreamAudio:} 成功发布音频流时，本地用户会收到该回调，此时本地用户可以自行选择是否调用 subscribeStreamAudio:subscribe:{@link #ByteRTCRoom#subscribeStreamAudio:subscribe:} 订阅此流。
 * @list 音视频传输
 * @order 2
 */

- (void)rtcRoom:(ByteRTCRoom* _Nonnull)rtcRoom onUserPublishStreamAudio:(NSString* _Nonnull)streamId info:(ByteRTCStreamInfo* _Nonnull)info isPublish:(BOOL)isPublish;

 /**
  * 
  * @type callback
  * @brief > 该接口将于 3.64 onRoomMessageReceived:uid:message:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomMessageReceived:uid:message:} 代替。
  * @brief 收到房间中调用 sendRoomMessage:{@link #ByteRTCRoom#sendRoomMessage:} 发送的广播文本消息时，收到此回调。
  * @param rtcRoom ByteRTCRoom 对象
  * @param uid 消息发送者 ID 。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRoomMessageReceived:(NSString *_Nonnull)uid
        message:(NSString *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief > 该接口将于 3.64 onRoomBinaryMessageReceived:uid:message:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomBinaryMessageReceived:uid:message:} 代替。
  * @brief 收到房间内广播二进制消息的回调。 <br>
  *        房间内其他用户调用 sendRoomBinaryMessage:{@link #ByteRTCRoom#sendRoomBinaryMessage:} 发送的广播二进制消息时，收到此回调。
  * @param rtcRoom ByteRTCRoom 对象
  * @param uid 消息发送者 ID 。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRoomBinaryMessageReceived:(NSString *_Nonnull)uid
        message:(NSData *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief > 该接口将于 3.64 onUserMessageReceived:uid:message:{@link #ByteRTCRoomDelegate#rtcRoom:onUserMessageReceived:uid:message:} 代替。
  * @brief 收到来自房间中其他用户通过 sendUserMessage:message:config:{@link #ByteRTCRTSRoom#sendUserMessage:message:config:} 发来的点对点文本消息时，会收到此回调。
  * @param rtcRoom ByteRTCRoom 对象
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onUserMessageReceived:(NSString *_Nonnull)uid
               message:(NSString *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief > 该接口将于 3.64 onUserBinaryMessageReceived:uid:message:{@link #ByteRTCRoomDelegate#rtcRoom:onUserBinaryMessageReceived:uid:message:} 代替。
  * @brief 收到来自房间中其他用户通过 sendUserBinaryMessage:message:config:{@link #ByteRTCRTSRoom#sendUserBinaryMessage:message:config:} 发来的点对点二进制消息时，会收到此回调。
  * @param rtcRoom ByteRTCRoom 对象
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onUserBinaryMessageReceived:(NSString *_Nonnull)uid
               message:(NSData *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到房间中调用 sendRoomMessage:{@link #ByteRTCRoom#sendRoomMessage:} 发送的广播文本消息时，收到此回调。
  * @param rtcRoom ByteRTCRoom 对象
  * @param msgid 消息编号。
  * @param uid 消息发送者 ID 。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRoomMessageReceived:(NSInteger)msgid uid:(NSString *_Nonnull)uid
        message:(NSString *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到房间内广播二进制消息的回调。 <br>
  *        房间内其他用户调用 sendRoomBinaryMessage:{@link #ByteRTCRoom#sendRoomBinaryMessage:} 发送的广播二进制消息时，收到此回调。
  * @param rtcRoom ByteRTCRoom 对象
  * @param msgid 消息编号。
  * @param uid 消息发送者 ID 。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRoomBinaryMessageReceived:(NSInteger)msgid uid:(NSString *_Nonnull)uid
        message:(NSData *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到来自房间中其他用户通过 sendUserMessage:message:config:{@link #ByteRTCRTSRoom#sendUserMessage:message:config:} 发来的点对点文本消息时，会收到此回调。
  * @param rtcRoom ByteRTCRoom 对象
  * @param msgid 消息编号。
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onUserMessageReceived:(NSInteger)msgid uid:(NSString *_Nonnull)uid
               message:(NSString *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到来自房间中其他用户通过 sendUserBinaryMessage:message:config:{@link #ByteRTCRTSRoom#sendUserBinaryMessage:message:config:} 发来的点对点二进制消息时，会收到此回调。
  * @param rtcRoom ByteRTCRoom 对象
  * @param msgid 消息编号.
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onUserBinaryMessageReceived:(NSInteger)msgid uid:(NSString *_Nonnull)uid
               message:(NSData *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 向房间内单个用户发送文本或二进制消息后（P2P），消息发送方会收到该消息发送结果回调。
  * @param rtcRoom ByteRTCRoom 对象。
  * @param msgid 本条消息的 ID。
  * @param error 消息发送结果，详见 ByteRTCUserMessageSendResult{@link #ByteRTCUserMessageSendResult}
  * @note
  *        - 你应调用 sendUserMessage:message:config:{@link #ByteRTCRTSRoom#sendUserMessage:message:config:} 向房间内单个用户发送文本消息
  *        - 你应调用 sendUserBinaryMessage:message:config:{@link #ByteRTCRTSRoom#sendUserBinaryMessage:message:config:} 向房间内单个用户发送二进制消息
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onUserMessageSendResult:(NSInteger)msgid error:(ByteRTCUserMessageSendResult)error;
 /**
  * 
  * @type callback
  * @brief 调用 sendRoomMessage:{@link #ByteRTCRoom#sendRoomMessage:} 或 sendRoomBinaryMessage:{@link #ByteRTCRoom#sendRoomBinaryMessage:} 向房间内群发文本或二进制消息后，消息发送方会收到该消息发送结果回调。
  * @param rtcRoom ByteRTCRoom 对象。
  * @param msgid 本条消息的 ID。
  * @param error 消息发送结果，详见 ByteRTCRoomMessageSendResult{@link #ByteRTCRoomMessageSendResult}
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRoomMessageSendResult:(NSInteger)msgid error:(ByteRTCRoomMessageSendResult)error;
/**
 * 
 * @valid since 3.52.
 * @type callback
 * @brief 调用 setRoomExtraInfo:value:{@link #ByteRTCRoom#setRoomExtraInfo:value:} 结果回调。
 * @param rtcRoom ByteRTCRoom 对象。
 * @param taskId 本次调用的任务编号。
 * @param result 设置房间附加信息结果，详见 ByteRTCSetRoomExtraInfoResult{@link #ByteRTCSetRoomExtraInfoResult}
 * @list 房间管理
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onSetRoomExtraInfoResult:(NSInteger)taskId result:(ByteRTCSetRoomExtraInfoResult)result;
/**
 * 
 * @valid since 3.52.
 * @type callback
 * @brief 接收到房间附加信息的回调。 <br>
 *        房间内其他用户调用 setRoomExtraInfo:value:{@link #ByteRTCRoom#setRoomExtraInfo:value:} 设置房间附加信息，收到此回调。 <br>
 *        另外用户加入房间成功后会收到这个房间全量附加信息
 * @param rtcRoom ByteRTCRoom 对象。
 * @param key 附加信息的键值
 * @param value 附加信息的内容
 * @param lastUpdateUserId 最后更新这条附加信息的用户编号
 * @param lastUpdateTimeMs 最后更新这条附加信息的 Unix 时间，时间精度是毫秒
 * @list 房间管理
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRoomExtraInfoUpdate:(NSString *_Nonnull)key value:(NSString *_Nonnull)value lastUpdateUserId:(NSString *_Nonnull)lastUpdateUserId lastUpdateTimeMs:(NSInteger)lastUpdateTimeMs;

/**
 * 
 * @valid since 3.54
 * @type callback
 * @brief 接收同一房间内，其他用户调用 setStreamExtraInfo:{@link #ByteRTCRoom#setStreamExtraInfo:} 设置的流附加信息的回调。
 * @param rtcRoom `ByteRTCRoom` 实例
 * @param streamId 流附加信息的流 ID
 * @param streamInfo 流附加信息的流信息
 * @param extraInfo 流附加信息
 * @list 音视频传输
 */
 
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onRoomStreamExtraInfoUpdate:(NSString *_Nonnull)streamId info:(ByteRTCStreamInfo *_Nonnull)streamInfo extraInfo:(NSString *_Nonnull)extraInfo;

/**
 * 
 * @valid since 3.54
 * @type callback
 * @brief 用户调用 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 设置用户可见性的回调。
 * @param rtcRoom `ByteRTCRoom` 实例
 * @param currentUserVisibility 当前用户的可见性。 <br>
 *        - YES: 可见，用户可以在房间内发布音视频流，房间中的其他用户将收到用户的行为通知，例如进房、开启视频采集和退房。
 *        - NO: 不可见，用户不可以在房间内发布音视频流，房间中的其他用户不会收到用户的行为通知，例如进房、开启视频采集和退房。
 * @param errorCode 设置用户可见性错误码，参看 ByteRTCUserVisibilityChangeError{@link #ByteRTCUserVisibilityChangeError}。
 * @list 房间管理
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onUserVisibilityChanged:(BOOL)currentUserVisibility errorCode:(ByteRTCUserVisibilityChangeError)errorCode;
/**
 * 
 * @type callback
 * @brief 通过调用服务端 BanUserStream/UnbanUserStream 方法禁用/解禁指定房间内指定用户视频流的发送时，触发此回调。
 * @param rtcRoom `ByteRTCRoom` 实例
 * @param uid 被禁用/解禁的视频流用户 ID
 * @param banned 视频流发送状态 <br>
 *        - true: 视频流发送被禁用
 *        - false: 视频流发送被解禁
 * @note
 *        - 房间内指定用户被禁止/解禁视频流发送时，房间内所有用户都会收到该回调。
 *        - 若被封禁用户断网或退房后再进房，则依然是封禁状态，且房间内所有人会再次收到该回调。
 *        - 指定用户被封禁后，房间内其他用户退房后再进房，会再次收到该回调。
 *        - 同一房间解散后再次创建，房间内状态清空。
 * @list 音视频传输
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onVideoStreamBanned:(NSString *_Nonnull)uid isBanned:(BOOL)banned;
/**
 * 
 * @type callback
 * @brief 通过调用服务端 BanUserStream/UnbanUserStream 方法禁用/解禁指定房间内指定用户音频流的发送时，触发此回调。
 * @param rtcRoom `ByteRTCRoom` 实例
 * @param uid 被禁用/解禁的音频流用户 ID
 * @param banned 音频流发送状态 <br>
 *        - true: 音频流发送被禁用
 *        - false: 音频流发送被解禁
 * @note
 *        - 房间内指定用户被禁止/解禁音频流发送时，房间内所有用户都会收到该回调。
 *        - 若被封禁用户断网或退房后再进房，则依然是封禁状态，且房间内所有人会再次收到该回调。
 *        - 指定用户被封禁后，房间内其他用户退房后再进房，会再次收到该回调。
 *        - 在控制台开启音频选路后，只有被封禁/解禁用户会收到该回调。
 *        - 同一房间解散后再次创建，房间内状态清空。
 * @list 音频管理
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onAudioStreamBanned:(NSString *_Nonnull)uid isBanned:(BOOL)banned;

#pragma mark ForwardStream related callback
    /**
     * 
     * @type callback
     * @brief 跨房间媒体流转发状态和错误回调
     * @param rtcRoom ByteRTCRoom 对象
     * @param infos 跨房间媒体流转发目标房间信息数组，详见 ByteRTCForwardStreamStateInfo{@link #ByteRTCForwardStreamStateInfo}
     * @list 音视频传输
     */
    
- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onForwardStreamStateChanged:(NSArray<ByteRTCForwardStreamStateInfo *> * _Nonnull)infos;
    /**
     * 
     * @type callback
     * @brief 跨房间媒体流转发事件回调
     * @param rtcRoom ByteRTCRoom 对象
     * @param infos 跨房间媒体流转发目标房间事件数组，详见 ByteRTCForwardStreamEventInfo{@link #ByteRTCForwardStreamEventInfo}
     * @list 音视频传输
     */
    
- (void)rtcRoom:( ByteRTCRoom * _Nonnull)rtcRoom onForwardStreamEvent:(NSArray<ByteRTCForwardStreamEventInfo *> * _Nonnull)infos;
/**
 * 
 * @type callback
 * @brief 加入房间并发布或订阅流后， 以每 2 秒一次的频率，报告本地用户和已订阅的远端用户的上下行网络质量信息。
 * @param rtcRoom ByteRTCRoom 对象
 * @param localQuality 本端网络质量，详见 ByteRTCNetworkQualityStats{@link #ByteRTCNetworkQualityStats}。
 * @param remoteQualities 已订阅用户的网络质量，详见 ByteRTCNetworkQualityStats{@link #ByteRTCNetworkQualityStats}。
 * @note 更多通话中的监测接口，详见[通话中质量监测](https:
 * @list 音视频传输
 */

- (void)rtcRoom:(ByteRTCRoom * _Nonnull)rtcRoom onNetworkQuality:(ByteRTCNetworkQualityStats *_Nonnull)localQuality remoteQualities:(NSArray<ByteRTCNetworkQualityStats*> *_Nonnull)remoteQualities;
/**
 * 
 * @type callback
 * @brief 字幕状态发生改变回调。 <br>
 *         当用户调用 startSubtitle:{@link #ByteRTCRoom#startSubtitle:} 和 stopSubtitle{@link #ByteRTCRoom#stopSubtitle} 使字幕状态发生改变或出现错误时，触发该回调。
 * @param rtcRoom ByteRTCRoom 实例。
 * @param state 字幕状态。参看 ByteRTCSubtitleState{@link #ByteRTCSubtitleState}。
 * @param errorCode 字幕任务错误码。参看 ByteRTCSubtitleErrorCode{@link #ByteRTCSubtitleErrorCode}。
 * @param errorMessage 第三方服务出现的错误。当因第三方服务出现错误，导致字幕状态改变时，用户可通过此参数获取具体的错误信息。如果不是因为第三方服务导致字幕状态改变，该字段为空。
 * @list 字幕翻译服务
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onSubtitleStateChanged:(ByteRTCSubtitleState)state
                                                             errorCode:(ByteRTCSubtitleErrorCode)errorCode
                                                          errorMessage:(NSString *_Nonnull)errorMessage;
/**
 * 
 * @type callback
 * @brief 字幕相关内容回调。 <br>
 *         当用户调用 startSubtitle:{@link #ByteRTCRoom#startSubtitle:} 后会收到此回调，通知字幕的相关信息。
 * @param rtcRoom ByteRTCRoom 实例。
 * @param subtitles 字幕消息内容。参看 ByteRTCSubtitleMessage{@link #ByteRTCSubtitleMessage}。
 * @list 字幕翻译服务
 */

- (void)rtcRoom:( ByteRTCRoom *_Nonnull)rtcRoom onSubtitleMessageReceived:(NSArray<ByteRTCSubtitleMessage*> * _Nonnull)subtitles;
@end
/**
 * 
 * @type api
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCRoom : ByteRTCRTSRoom
/**
 * 
 * @type callback
 */

@property(nonatomic, weak) id<ByteRTCRoomDelegate> _Nullable delegate;

#pragma mark - Core Methods
/**
 * 
 * @type api
 * @brief 退出并销毁调用 createRTCRoom:{@link #ByteRTCEngine#createRTCRoom:} 所创建的房间实例。
 * @list 房间管理
 */

- (void)destroy;
/**
 * 
 * @valid since 3.53
 * @type api
 * @brief 获取房间 ID。
 * @return 房间 ID。
 * @list 房间管理
 */

- (NSString *_Nonnull)getRoomId;
/**
 * 
 * @type api
 * @brief 通过设置 ByteRTCRoom{@link #ByteRTCRoom} 对象的事件句柄，监听此对象对应的回调事件。
 * @param roomDelegate 参见 ByteRTCRoomDelegate{@link #ByteRTCRoomDelegate}。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @list 房间管理
 */


- (int)setRTCRoomDelegate:(id<ByteRTCRoomDelegate> _Nullable)roomDelegate;
/**
 * 
 * @type api
 * @brief 加入 RTC 房间。 <br>
 *        调用 createRTCRoom:{@link #ByteRTCEngine#createRTCRoom:} 创建房间后，调用此方法加入房间，同房间内其他用户进行音视频通话。
 * @param token 动态密钥，用于对进房用户进行鉴权验证。 <br>
 *        进入房间需要携带 Token。测试时可使用控制台生成临时 Token，正式上线需要使用密钥 SDK 在你的服务端生成并下发 Token。Token 有效期及生成方式参看[使用 Token 完成鉴权](70121)。 <br>
 *        使用不同 AppID 的 App 是不能互通的。 <br>
 *        请务必保证生成 Token 使用的 AppID 和创建引擎时使用的 AppID 相同，否则会导致加入房间失败。
 * @param userInfo 用户信息。参看 ByteRTCUserInfo{@link #ByteRTCUserInfo}。
 * @param userVisibility 用户可见性。建议在进房时将用户可见性都设置为 `false`，并在用户需要发送音视频流时再通过 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 设置为 `true`。从而避免因房间内用户达到数量上限所导致的进房失败。默认情况下，一个 RTC 房间最多同时容纳 50 名可见用户，其中最多 30 人可同时上麦，更多信息参看[用户和媒体流上限](https:
 * @param roomConfig 房间参数配置，设置房间模式以及是否自动发布或订阅流。具体配置模式参看 ByteRTCRoomConfig{@link #ByteRTCRoomConfig}。
 * @return 方法调用结果。 <br>
 *        -  0: 成功。触发以下回调：
 *          - 本端收到房间状态通知 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调。
 *          - 本端收到本地流发布状态通知 rtcRoom:onVideoPublishStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onVideoPublishStateChanged:info:state:reason:}、rtcRoom:onAudioPublishStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onAudioPublishStateChanged:info:state:reason:}。
 *          - 本端收到流订阅状态通知 rtcRoom:onVideoSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onVideoSubscribeStateChanged:info:state:reason:}、rtcRoom:onAudioSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onAudioSubscribeStateChanged:info:state:reason:}。
 *          - 本端收到房间内已发布流的通知 rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}、rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:}。 
 *          - 如果本端用户为可见用户，房间内其他用户收到 rtcRoom:onUserJoined:{@link #ByteRTCRoomDelegate#rtcRoom:onUserJoined:} 回调通知。
 *        - -1: 参数无效
 *        - -2: 已经在房间内。接口调用成功后，只要收到返回值为 0 ，且未调用 leaveRoom{@link #ByteRTCRoom#leaveRoom} 成功，则再次调用进房接口时，无论填写的房间 ID 和用户 ID 是否重复，均触发此返回值。
 *        调用失败时，具体失败原因会通过 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调告知。
 * @note
 *        - 同一个 AppID 的同一个房间内，每个用户的用户 ID 必须是唯一的。如果两个用户的用户 ID 相同，则后加入房间的用户会将先加入房间的用户踢出房间，并且先加入房间的用户会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调通知，错误类型为重复登录 ByteRTCErrorCodeDuplicateLogin。
 *        - 本地用户调用此方法加入房间成功后，会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调通知。若本地用户同时为可见用户，加入房间时远端用户会收到 rtcRoom:onUserJoined:{@link #ByteRTCRoomDelegate#rtcRoom:onUserJoined:} 回调通知。
 *        - 房间内不可见用户的容量远远大于可见用户，而且用户默认可见，因此对于不参与互动的用户，你需要调用 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 更改为不可见用户。从而避免因房间内用户达到数量上限所导致的进房失败。默认情况下，一个 RTC 房间最多同时容纳 50 名可见用户，其中最多 30 人可同时上麦，更多信息参看[用户和媒体流上限](https:
 *        - 用户加入房间成功后，在本地网络状况不佳的情况下，SDK 可能会与服务器失去连接，并触发 rtcEngine:onConnectionStateChanged:{@link #ByteRTCEngineDelegate#rtcEngine:onConnectionStateChanged:} 回调。此时 SDK 会自动重试，直到成功重连。重连成功后，本地会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:}。
 * @list 房间管理
 */

- (int)joinRoom:(NSString *_Nullable)token userInfo:(ByteRTCUserInfo *_Nonnull)userInfo userVisibility:(BOOL)userVisibility roomConfig:(ByteRTCRoomConfig *_Nonnull)roomConfig NS_SWIFT_NAME(joinRoom(_:userInfo:userVisibility:roomConfig:));
/**
 * 
 * @type api
 * @brief 设置用户可见性。未调用该接口前，本地用户默认对他人可见。 <br>
 *        默认情况下，一个 RTC 房间最多同时容纳 50 名可见用户，最多 30 人可同时上麦。更多信息参看[用户和媒体流上限](https:
 * @param enable 设置用户是否对房间内其他用户可见： <br>
 *        - YES: 可见，用户可以在房间内发布音视频流，房间中的其他用户将收到用户的行为通知，例如进房、开启视频采集和退房。
 *        - NO: 不可见，用户不可以在房间内发布音视频流，房间中的其他用户不会收到用户的行为通知，例如进房、开启视频采集和退房。
 * @return
 *        - 0: 调用成功。
 *        - < 0: 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 *       设置用户可见性，会收到设置成功/失败回调 rtcRoom:onUserVisibilityChanged:errorCode:{@link #ByteRTCRoomDelegate#rtcRoom:onUserVisibilityChanged:errorCode:}。（v3.54 新增）
 *        - 在加入房间前设置用户可见性，若设置的可见性与默认值不同，将在加入房间时触发本回调。 <br>
 *        - 在加入房间后设置用户可见性，若可见性前后不同，会触发本回调。 <br>
 *        - 在断网重连后，若可见性发生改变，会触发本回调。 <br>
 * @note
 *       - 在加入房间前后，用户均可调用此方法设置用户可见性。
 *       - 在房间内，调用此方法成功切换用户可见性后，房间内其他用户会收到相应的回调。
 *       - 从可见换至不可见时，房间内其他用户会收到 rtcRoom:onUserLeave:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onUserLeave:reason:}。
 *       - 从不可见切换至可见时，房间内其他用户会收到 rtcRoom:onUserJoined:{@link #ByteRTCRoomDelegate#rtcRoom:onUserJoined:}。
 *       - 若调用该方法将可见性设为 `false`，此时尝试发布流会收到 `ByteRTCWarningCodeSubscribeStreamForbiden` 警告。
 * @list 房间管理
 */

- (int)setUserVisibility:(BOOL)enable;
/**
 * 
 * @type api
 * @brief 设置发流端音画同步。 <br>
 *        当同一用户同时使用两个通话设备分别采集发送音频和视频时，有可能会因两个设备所处的网络环境不一致而导致发布的流不同步，此时你可以在视频发送端调用该接口，SDK 会根据音频流的时间戳自动校准视频流，以保证接收端听到音频和看到视频在时间上的同步性。
 * @param audioUserId 音频发送端的用户 ID，将该参数设为空则可解除当前音视频的同步关系。
 * @return
 *        - 0: 调用成功。调用该接口后音画同步状态发生改变时，你会收到 rtcRoom:onAVSyncStateChange:{@link #ByteRTCRoomDelegate#rtcRoom:onAVSyncStateChange:} 回调。
 *        - < 0 : 调用失败。监听 rtcRoom:onAVSyncEvent:userId:eventCode:{@link #ByteRTCRoomDelegate#rtcRoom:onAVSyncEvent:userId:eventCode:} 获取错误详情。同一 RTC 房间内允许存在多个音视频同步关系，但需注意单个音频源不支持与多个视频源同时同步。
 * @note
 *        - 该方法在进房前后均可调用。
 *        - 进行音画同步的音频发布用户 ID 和视频发布用户 ID 须在同一个 RTC 房间内。
 *        - 如需更换同步音频源，再次调用该接口传入新的 `audioUserId` 即可；如需更换同步视频源，需先解除当前的同步关系，后在新视频源端开启同步。
 * @list 音视频传输
 */

- (int)setMultiDeviceAVSync:(NSString* _Nullable) audioUserId;
/**
 * 
 * @type api
 * @brief 离开 RTC 房间。 <br>
 *        调用此方法结束通话过程，并释放所有通话相关的资源。
 * @return
 *        - 0: 调用成功。如果用户是房间内可见用户，触发以下回调：
 *            - 远端用户收到 rtcRoom:onUserLeave:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onUserLeave:reason:} 回调通知。
 *            - 正在发布的流会被取消发布。远端用户收到 rtcRoom:onVideoPublishStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onVideoPublishStateChanged:info:state:reason:}、rtcRoom:onAudioPublishStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onAudioPublishStateChanged:info:state:reason:} 回调通知。
 *        - < 0: 调用失败，参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 * @note
 *       - 加入房间后，必须调用此方法结束通话，否则无法开始下一次通话。
 *       - 此方法是异步操作，调用返回时并没有真正退出房间。真正退出房间后，本地会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调通知。你必须在收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调后，再销毁房间或引擎，或调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 再次加入房间。  
 *       - 调用 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 将自身设为可见的用户离开房间后，房间内其他用户会收到 rtcRoom:onUserLeave:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onUserLeave:reason:} 回调通知。
 * @list 房间管理
 */

- (int)leaveRoom NS_SWIFT_NAME(leaveRoom());

/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `publishStream`、`unpublishStream` 、`publishScreen` 和 `unpublishScreen` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，并且仍在使用这些方法，请迁移到此接口。
 * @brief 发布/取消发布视频流。
 * @param publish 是否发布视频流。<br>
 *        - true: 发布。
 *        - false: 取消发布。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 如果你已经在用户进房时通过调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 成功选择了自动发布，则无需再调用本接口。
 *        - 调用 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 方法将自身设置为不可见后无法调用该方法，需将自身切换至可见后方可调用该方法发布摄像头视频流。
 *        - 如果你需要发布麦克风采集到的音频流，调用 publishStreamAudio:{@link #ByteRTCRoom#publishStreamAudio:}。
 *        - 如果你需要向多个房间发布流，调用 startForwardStreamToRooms:{@link #ByteRTCRoom#startForwardStreamToRooms:}。
 *        - 调用此方法后，房间中的所有远端用户会收到 rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:} 回调通知，订阅了视频流的远端用户会收到 rtcEngine:onFirstRemoteVideoFrameDecoded:info:withFrameInfo:{@link #ByteRTCEngineDelegate#rtcEngine:onFirstRemoteVideoFrameDecoded:info:withFrameInfo:} 回调。
 * @list 音视频传输
 */

- (int)publishStreamVideo:(BOOL)publish;

/**
  * 
  * @type api
  * @valid since 3.60. 自 3.60 起，该接口替代了 `publishStream`、`unpublishStream` 、`publishScreen` 和 `unpublishScreen` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，并且仍在使用这些方法，请迁移到此接口。
  * @brief 发布/取消发布音频流。
  * @param publish 指定是否发布音频流。<br>
  *        - true: 发布。
  *        - false: 取消发布。
  * @return
  *        - 0: 调用成功。
  *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
  * @note
  *        - 如果你已经在用户进房时通过调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 成功选择了自动发布，则无需再调用本接口。
  *        - 调用 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 方法将自身设置为不可见后无法调用该方法，需将自身切换至可见后方可调用该方法发布音频流。
  *        - 如果你需要发布摄像头采集到的视频流，调用 publishStreamVideo:{@link #ByteRTCRoom#publishStreamVideo:}。
  *        - 如果你需要向多个房间发布流，调用 startForwardStreamToRooms:{@link #ByteRTCRoom#startForwardStreamToRooms:}。
  *        - 调用此方法后，房间中的所有远端用户会收到 rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:} 回调通知，其中成功收到了音频流的远端用户会收到 rtcEngine:onFirstRemoteAudioFrame:info:{@link #ByteRTCEngineDelegate#rtcEngine:onFirstRemoteAudioFrame:info:} 回调。
  * @list 音视频传输
  * @order 0
  */

- (int)publishStreamAudio:(BOOL)publish;

/**
  * 
  * @type api
  * @valid since 3.60. 自 3.60 起，该接口替代了 `subscribeStream` 和 `unsubscribeStream` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，且仍在使用这两个方法，请迁移至该接口。
  * @brief 订阅/取消订阅房间内指定的远端视频流。
  * @param streamId 目标远端视频流 ID。
  * @param subscribe 指定是否订阅该视频流。<br>
  *        - true: 订阅。
  *        - false: 取消订阅。
  * @return 方法调用结果： <br>
  *        - 0：成功；
  *        - <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
  * @note
  *        - 若当前用户在调用本接口时已经订阅该远端用户（手动订阅或自动订阅），则将根据本次传入的参数，更新订阅配置。
  *        - 你必须先通过 rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:} 回调获取当前房间里的远端摄像头流信息，然后调用本方法按需订阅。
  *        - 调用该方法后，你会收到 rtcRoom:onVideoSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onVideoSubscribeStateChanged:info:state:reason:} 通知方法调用结果。
  *        - 成功订阅远端用户的媒体流后，订阅关系将持续到调用本方法取消订阅或本端用户退房。
  *        - 关于其他调用异常，你会收到 rtcRoom:onVideoSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onVideoSubscribeStateChanged:info:state:reason:} 回调通知，具体异常原因参看 ByteRTCSubscribeStateChangeReason{@link #ByteRTCSubscribeStateChangeReason}。
  * @list 音视频传输
  * @order 3
  */

- (int)subscribeStreamVideo:(NSString *_Nonnull)streamId subscribe:(BOOL)subscribe;

/**
  * 
  * @type api
  * @valid since 3.60. 自 3.60 起，该接口替代了 `subscribeStream` 和 `unsubscribeStream` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，且仍在使用这两个方法，请迁移至该接口。
  * @brief 订阅/取消订阅房间内指定的远端音频流。
  * @param streamId 目标远端音频流 ID。
  * @param subscribe 指定是否订阅该音频流。<br>
  *        - true: 订阅。
  *        - false: 取消订阅。
  * @return 方法调用结果： <br>
  *        - 0：成功；
  *        - <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
  * @note
  *        - 若当前用户在调用本接口时已经订阅该远端用户（手动订阅或自动订阅），则将根据本次传入的参数，更新订阅配置。
  *        - 你必须先通过 rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:} 回调获取当前房间里的远端麦克风流信息，然后调用本方法按需订阅。
  *        - 调用该方法后，你会收到 rtcRoom:onAudioSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onAudioSubscribeStateChanged:info:state:reason:} 通知方法调用结果。
  *        - 成功订阅远端用户的媒体流后，订阅关系将持续到调用 subscribeStreamAudio:subscribe:{@link #ByteRTCRoom#subscribeStreamAudio:subscribe:} 取消订阅或本端用户退房。
  *        - 关于其他调用异常，你会收到 rtcRoom:onAudioSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onAudioSubscribeStateChanged:info:state:reason:} 回调通知，具体异常原因参看 ByteRTCSubscribeStateChangeReason{@link #ByteRTCSubscribeStateChangeReason}。
  * @list 音视频传输
  * @order 3
  */

- (int)subscribeStreamAudio:(NSString *_Nonnull)streamId subscribe:(BOOL)subscribe;

/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `subscribeAllStreams` 和 `unsubscribeAllStreams` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，且仍在使用这两个方法，请迁移至该接口。
 * @brief 订阅/取消订阅房间内所有远端视频流（通过摄像头采集的）。
 * @param subscribe  是否订阅所有远端视频流。<br>
 *        - true: 订阅。
 *        - false: 取消订阅。
 * @return
 *        0: 方法调用成功 <br>
 *        !0: 方法调用失败
 * @note
 *        - 多次调用订阅接口时，将根据末次调用接口和传入的参数，更新订阅配置。
 *        - 开启音频选路后，如果房间内的媒体流超过上限，建议通过调用 subscribeStreamVideo:subscribe:{@link #ByteRTCRoom#subscribeStreamVideo:subscribe:} 接口逐一指定需要订阅的媒体流。
 *        - 调用该方法后，你会收到 rtcRoom:onVideoSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onVideoSubscribeStateChanged:info:state:reason:} 通知方法调用结果。
 *        - 成功调用本接口后，订阅关系将持续到调用 subscribeAllStreamsVideo:{@link #ByteRTCRoom#subscribeAllStreamsVideo:}  取消订阅或本端用户退房。
 *        - 关于其他调用异常，你会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调通知，具体异常原因参看 ByteRTCErrorCode{@link #ByteRTCErrorCode}。
 * @list 音视频传输
 */

- (int)subscribeAllStreamsVideo:(BOOL)subscribe;


/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `subscribeAllStreams` 和 `unsubscribeAllStreams` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，且仍在使用这两个方法，请迁移至该接口。
 * @brief 订阅或取消订阅所有远端音频流（通过麦克风采集的）。
 * @param subscribe 是否订阅所有远端音频流。<br>
 *        - true: 订阅。
 *        - false: 取消订阅。
 * @return
 *        - 0: 成功。
 *        - < 0: 失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 * @note
 *        - 多次调用订阅接口时，将根据末次调用接口和传入的参数，更新订阅配置。
 *        - 开启音频选路后，如果房间内的媒体流超过上限，建议通过调用 subscribeStreamAudio:subscribe:{@link #ByteRTCRoom#subscribeStreamAudio:subscribe:} 接口逐一指定需要订阅的媒体流。
 *        - 调用该方法后，你会收到 rtcRoom:onAudioSubscribeStateChanged:info:state:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onAudioSubscribeStateChanged:info:state:reason:} 通知方法调用结果。
 *        - 成功订阅远端用户的媒体流后，订阅关系将持续到调用 subscribeStreamAudio:subscribe:{@link #ByteRTCRoom#subscribeStreamAudio:subscribe:} 取消订阅或本端用户退房。
 *        - 关于其他调用异常，你会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调通知，具体异常原因参看 ByteRTCErrorCode{@link #ByteRTCErrorCode}。
 * @list 音视频传输
 */

- (int)subscribeAllStreamsAudio:(BOOL)subscribe;

/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `resumeAllSubscribedStream` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，且仍在使用该方法，请迁移至该接口。
 * @brief 暂停接收所有远端视频流。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 * @note
 *        - 该方法仅暂停远端流的接收，并不影响远端流的采集和发送；
 *        - 该方法不改变用户的订阅状态以及订阅流的属性。
 *        - 若想恢复接收远端流，需调用 resumeAllSubscribedStreamVideo{@link #ByteRTCRoom#resumeAllSubscribedStreamVideo}。
 *        - 多房间场景下，仅暂停接收发布在当前所在房间的流。
 * @list 音视频传输
 */

- (int)pauseAllSubscribedStreamVideo;

/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `pauseAllSubscribedStream` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，且仍在使用该方法，请迁移至该接口。
 * @brief 暂停接收所有远端音频流。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 该方法仅暂停远端流的接收，并不影响远端流的采集和发送；
 *        - 该方法不改变用户的订阅状态以及订阅流的属性。
 *        - 若想恢复接收远端流，需调用 resumeAllSubscribedStreamVideo{@link #ByteRTCRoom#resumeAllSubscribedStreamVideo}。
 *        - 多房间场景下，仅暂停接收发布在当前所在房间的流。
 * @list 音视频传输
 */

- (int)pauseAllSubscribedStreamAudio;

/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `resumeAllSubscribedStream` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，且仍在使用该方法，请迁移至该接口。
 * @brief 恢复接收所有远端视频流。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 * @note
 *        - 该方法仅恢复远端流的接收，并不影响远端流的采集和发送；
 *        - 该方法不改变用户的订阅状态以及订阅流的属性。
 * @list 音视频传输
 */

- (int)resumeAllSubscribedStreamVideo;

/**
 * 
 * @type api
 * @valid since 3.60. 自 3.60 起，该接口替代了 `resumeAllSubscribedStream` 方法来实现下述功能。如果你已升级至 3.60 及以上版本，且仍在使用该方法，请迁移至该接口。
 * @brief 恢复接收所有远端音频流。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note
 *        - 该方法仅恢复远端流的接收，并不影响远端流的采集和发送；
 *        - 该方法不改变用户的订阅状态以及订阅流的属性。
 * @list 音视频传输
 */

- (int)resumeAllSubscribedStreamAudio;

 /**
  * 
  * @type api
  * @brief 给房间内的所有其他用户群发文本消息。
  * @param message <br>
  *        发送的文本消息内容。 <br>
  *        消息不超过 64 KB。
  * @return 这次发送消息的编号，从 1 开始递增。
  * @note
  *      - 在发送房间内文本消息前，必须先调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 加入房间。
  *      - 调用该函数后会收到一次 rtcRoom:onRoomMessageSendResult:error:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomMessageSendResult:error:} 回调，通知消息发送方发送成功或失败；
  *      - 若文本消息发送成功，则房间内所有远端用户会收到 rtcRoom:onRoomMessageReceived:message:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomMessageReceived:message:} 回调。
  * @list 消息
  */
 
- (NSInteger)sendRoomMessage:(NSString *_Nonnull)message NS_SWIFT_NAME(sendRoomMessage(_:));
 /**
  * 
  * @type api
  * @brief 给房间内的所有其他用户群发二进制消息。
  * @param message <br>
  *        用户发送的二进制广播消息 <br>
  *        消息不超过 64KB。
  * @return 这次发送消息的编号，从 1 开始递增。
  * @note
  *      - 在房间内广播二进制消息前，必须先调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 加入房间。
  *      - 调用该函数后会收到一次 rtcRoom:onRoomMessageSendResult:error:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomMessageSendResult:error:} 回调，通知消息发送方发送成功或失败；
  *      - 若二进制消息发送成功，则房间内所有用户会收到 rtcRoom:onRoomBinaryMessageReceived:message:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomBinaryMessageReceived:message:} 回调。
  * @list 消息
  */
 
- (NSInteger)sendRoomBinaryMessage:(NSData *  _Nonnull)message NS_SWIFT_NAME(sendRoomBinaryMessage(_:));

#pragma mark ForwardStream
    /**
     * 
     * @type api
     * @brief 开始跨房间转发媒体流。 <br>
     *        在调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 后调用本接口，实现向多个房间转发媒体流，适用于跨房间连麦等场景。
     * @param configurations 跨房间媒体流转发指定房间的信息。参看 ByteRTCForwardStreamConfiguration{@link #ByteRTCForwardStreamConfiguration}。
     * @return
     *        - 0: 成功。
     *        - < 0: 失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
     * @note
     *        - 调用本方法后，将在本端触发 rtcRoom:onForwardStreamStateChanged:{@link #ByteRTCRoomDelegate#rtcRoom:onForwardStreamStateChanged:} 回调。
     *        - 调用本方法后，你可以通过监听 rtcRoom:onForwardStreamEvent:{@link #ByteRTCRoomDelegate#rtcRoom:onForwardStreamEvent:} 回调来获取各个目标房间在转发媒体流过程中的相关事件。
     *        - 开始转发后，目标房间中的用户将接收到本地用户进房 rtcRoom:onUserJoined:{@link #ByteRTCRoomDelegate#rtcRoom:onUserJoined:} 和发流 rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}、rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:} 回调。
     *        - 调用本方法后，可以调用 updateForwardStreamToRooms:{@link #ByteRTCRoom#updateForwardStreamToRooms:} 更新目标房间信息，例如，增加或减少目标房间等。
     *        - 调用本方法后，可以调用 stopForwardStreamToRooms{@link #ByteRTCRoom#stopForwardStreamToRooms} 停止向所有房间转发媒体流。
     *        - 调用本方法后，可以调用 pauseForwardStreamToAllRooms{@link #ByteRTCRoom#pauseForwardStreamToAllRooms} 暂停向所有房间转发媒体流。
     * @list 音视频传输
     */
    
- (int)startForwardStreamToRooms:(NSArray <ByteRTCForwardStreamConfiguration *> * _Nullable)configurations NS_SWIFT_NAME(startForwardStreamToRooms(_:));
    /**
     * 
     * @type api
     * @brief 更新跨房间媒体流转发信息。 <br>
     *        通过 startForwardStreamToRooms:{@link #ByteRTCRoom#startForwardStreamToRooms:} 发起媒体流转发后，可调用本方法增加或者减少目标房间，或更新房间密钥。 <br>
     *        调用本方法增加或删减房间后，将在本端触发 rtcRoom:onForwardStreamStateChanged:{@link #ByteRTCRoomDelegate#rtcRoom:onForwardStreamStateChanged:} 回调，包含发生了变动的目标房间中媒体流转发状态。
     * @param configurations 跨房间媒体流转发目标房间信息。参看 ByteRTCForwardStreamConfiguration{@link #ByteRTCForwardStreamConfiguration}。
     * @return
     *        - 0: 成功。
     *        - < 0: 失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
     * @note
     *        增加或删减目标房间后，新增目标房间中的用户将接收到本地用户进房 rtcRoom:onUserJoined:{@link #ByteRTCRoomDelegate#rtcRoom:onUserJoined:} 和发布 rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}、rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:} 回调。
     * @list 音视频传输
     */
    
- (int)updateForwardStreamToRooms:(NSArray <ByteRTCForwardStreamConfiguration *> * _Nullable)configurations NS_SWIFT_NAME(updateForwardStreamToRooms(_:));
/**
 * 
 * @type api
 * @brief 停止跨房间媒体流转发。 <br>
 *        通过 startForwardStreamToRooms:{@link #ByteRTCRoom#startForwardStreamToRooms:} 发起媒体流转发后，可调用本方法停止向所有目标房间转发媒体流。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 * @note
 *        - 调用本方法后，将在本端触发 rtcRoom:onForwardStreamStateChanged:{@link #ByteRTCRoomDelegate#rtcRoom:onForwardStreamStateChanged:} 回调。
 *        - 调用本方法后，原目标房间中的用户将接收到本地用户停止发布 rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}、rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:} 回调和退房 rtcRoom:onUserLeave:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onUserLeave:reason:} 的回调。
 *        - 如果需要更改目标房间，请调用 updateForwardStreamToRooms:{@link #ByteRTCRoom#updateForwardStreamToRooms:} 更新房间信息。
 *        - 如果需要暂停转发，请调用 pauseForwardStreamToAllRooms{@link #ByteRTCRoom#pauseForwardStreamToAllRooms}，并在之后随时调用 resumeForwardStreamToAllRooms{@link #ByteRTCRoom#resumeForwardStreamToAllRooms} 快速恢复转发。
 * @list 音视频传输
 */

- (int)stopForwardStreamToRooms;
/**
 * 
 * @type api
 * @brief 暂停跨房间媒体流转发。 <br>
 *        通过 startForwardStreamToRooms:{@link #ByteRTCRoom#startForwardStreamToRooms:} 发起媒体流转发后，可调用本方法暂停向所有目标房间转发媒体流。 <br>
 *        调用本方法暂停向所有目标房间转发后，你可以随时调用 resumeForwardStreamToAllRooms{@link #ByteRTCRoom#resumeForwardStreamToAllRooms} 快速恢复转发。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 * @note 调用本方法后，目标房间中的用户将接收到本地用户停止发布 rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}、rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:} 回调和退房 rtcRoom:onUserLeave:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onUserLeave:reason:} 的回调。
 * @list 音视频传输
 * @order 13
 */
  
- (int)pauseForwardStreamToAllRooms;
/**
 * 
 * @type api
 * @brief 恢复跨房间媒体流转发。 <br>
 *        调用 pauseForwardStreamToAllRooms{@link #ByteRTCRoom#pauseForwardStreamToAllRooms} 暂停转发之后，调用本方法恢复向所有目标房间转发媒体流。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 * @note
 *        目标房间中的用户将接收到本地用户进房 rtcRoom:onUserJoined:{@link #ByteRTCRoomDelegate#rtcRoom:onUserJoined:} 和发布 rtcRoom:onUserJoined:{@link #ByteRTCRoomDelegate#rtcRoom:onUserJoined:} 的回调。
 * @list 音视频传输
 */

- (int)resumeForwardStreamToAllRooms;
/**
 * 
 * @type api
 * @brief 获取范围语音接口实例。
 * @return 方法调用结果： <br>
 *        - ByteRTCRangeAudio：成功，返回一个 ByteRTCRangeAudio{@link #ByteRTCRangeAudio} 实例。
 *        - NULL：失败，当前 SDK 不支持范围语音功能。
 * @note 首次调用该方法须在创建房间后、加入房间前。范围语音相关 API 和调用时序详见[范围语音](https:
 * @list 高级功能
 */

- (ByteRTCRangeAudio *_Nullable)getRangeAudio;
/**
 * 
 * @type api
 * @brief 获取空间音频接口实例。
 * @return 方法调用结果： <br>
 *        - ByteRTCSpatialAudio：成功，返回一个 ByteRTCSpatialAudio{@link #ByteRTCSpatialAudio} 实例。
 *        - NULL：失败，当前 SDK 不支持空间音频功能。
 * @note
 *        - 首次调用该方法须在创建房间后、加入房间前。 空间音频相关 API 和调用时序详见[空间音频](https:
 *        - 只有在使用支持真双声道播放的设备时，才能开启空间音频效果；
 *        - 机型性能不足可能会导致音频卡顿，使用低端机时，不建议开启空间音频效果；
 *        - SDK 最多支持 30 个用户同时开启空间音频功能。
 * @list 高级功能
 */

- (ByteRTCSpatialAudio *_Nullable)getSpatialAudio;
/**
 * 
 * @type api
 * @brief 调节某个房间内所有远端用户的音频播放音量。
 * @param volume 音频播放音量值和原始音量的比值，范围是 [0, 400]，单位为 %，自带溢出保护。为保证更好的通话质量，建议将 volume 值设为 [0,100]。 <br>
 *              - 0: 静音
 *              - 100: 原始音量，默认值
 *              - 400: 最大可为原始音量的 4 倍(自带溢出保护)
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note 假设某远端用户 A 始终在被调节的目标用户范围内， <br>
 *        - 该方法与 setRemoteAudioPlaybackVolume:volume:{@link #ByteRTCEngine#setRemoteAudioPlaybackVolume:volume:} 互斥，最新调用的任一方法设置的音量将覆盖此前已设置的音量，效果不叠加；
 *        - 当该方法与 setPlaybackVolume:{@link #ByteRTCEngine#setPlaybackVolume:} 方法共同使用时，本地收听用户 A 的音量将为两次设置的音量效果的叠加。
 * @list 音频管理
 */

- (int)setRemoteRoomAudioPlaybackVolume:(NSInteger)volume;
/**
 * 
 * @valid since 3.52.
 * @type api
 * @brief 设置本端发布流在音频选路中的优先级。
 * @param audioSelectionPriority 本端发布流在音频选路中的优先级，默认正常参与音频选路。参见 ByteRTCAudioSelectionPriority{@link #ByteRTCAudioSelectionPriority}。
 * @note
 * 在控制台上为本 appId 开启音频选路后，调用本接口才会生效。进房前后调用均可生效。更多信息参见[音频选路](https:
 * 如果本端用户同时加入不同房间，使用本接口进行的设置相互独立。
 * @list 音频管理
 */

- (int)setAudioSelectionConfig:(ByteRTCAudioSelectionPriority)audioSelectionPriority;
/**
 * 
 * @valid since 3.52.
 * @type api
 * @brief 设置/更新房间附加信息，可用于标识房间状态或属性，或灵活实现各种业务逻辑。
 * @param key 房间附加信息键值，长度小于 10 字节。 <br>
 *        同一房间内最多可存在 5 个 key，超出则会从第一个 key 起进行替换。
 * @param value 房间附加信息内容，长度小于 128 字节。
 * @return
 *        - 0: 方法调用成功，返回本次调用的任务编号；
 *        - <0: 方法调用失败，具体原因详见 ByteRTCSetRoomExtraInfoResult{@link #ByteRTCSetRoomExtraInfoResult}。
 * @note
 *       - 在设置房间附加信息前，必须先调用 joinRoom:userInfo:userVisibility:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:userVisibility:roomConfig:} 加入房间。
 *       - 调用该方法后，会收到一次 rtcRoom:onSetRoomExtraInfoResult:result:{@link #ByteRTCRoomDelegate#rtcRoom:onSetRoomExtraInfoResult:result:} 回调，提示设置结果。
 *       - 调用该方法成功设置附加信息后，同一房间内的其他用户会收到关于该信息的回调 rtcRoom:onRoomExtraInfoUpdate:value:lastUpdateUserId:lastUpdateTimeMs:{@link #ByteRTCRoomDelegate#rtcRoom:onRoomExtraInfoUpdate:value:lastUpdateUserId:lastUpdateTimeMs:}。
 *       - 新进房的用户会收到进房前房间内已有的全部附加信息通知。
 * @list 房间管理
 */

- (NSInteger)setRoomExtraInfo:(NSString *_Nonnull)key value:(NSString *_Nonnull)value;

/**
 * 
 * @type api
 * @region 流管理
 * @brief 设置当前推流的流附加信息。
 * @param extra_info 流附加信息。长度不超过1024的字符串。
 * @return 方法调用结果： <br>
 *        + 0：成功；<br>
 *        + !0：失败。
 * @note 
 *        + 可通过此函数设置当前推流的流附加信息。流附加信息是流 ID 的附加信息标识，不同于流 ID 在推流过程中不可修改，流附加信息可以在对应流 ID 的推流中途修改。开发者可根据流附加信息来实现流 ID 相关的可变内容的同步。
 *        + 该方法在进房前后均可调用
 *        + 相同房间内的其他用户会通过 [onRoomStreamExtraInfoUpdate] 回调函数获得通知。
 * @list 音视频传输
 */ 

- (NSInteger)setStreamExtraInfo:(NSString *_Nonnull)extra_info;

/**
 * 
 * @valid since 3.52
 * @type api
 * @brief 识别或翻译房间内所有用户的语音，形成字幕。 <br>
 *        调用该方法时，可以在 ByteRTCSubtitleMode{@link #ByteRTCSubtitleMode} 中选择语音识别或翻译模式。如果选择识别模式，语音识别文本会通过 rtcRoom:onSubtitleMessageReceived:{@link #ByteRTCRoomDelegate#rtcRoom:onSubtitleMessageReceived:} 事件回调给你； <br>
 *        如果选择翻译模式，你会同时收到两个 rtcRoom:onSubtitleMessageReceived:{@link #ByteRTCRoomDelegate#rtcRoom:onSubtitleMessageReceived:} 回调，分别包含字幕原文及字幕译文。 <br>
 *        调用该方法后，用户会收到 rtcRoom:onSubtitleStateChanged:errorCode:errorMessage:{@link #ByteRTCRoomDelegate#rtcRoom:onSubtitleStateChanged:errorCode:errorMessage:} 回调，通知字幕是否开启。
 * @param subtitleConfig 字幕配置信息。参看 ByteRTCSubtitleConfig{@link #ByteRTCSubtitleConfig}。
 * @return
 *        - 0: 成功。
 *        - < 0: 失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 * @note
 *        - 使用字幕功能前，你需要在 [RTC 控制台](https:
 *        - 如果你需要使用流式语音识别模式，你应在 [语音技术控制台](https:
 *        - 如果你需要使用实时语音翻译模式，你应开通机器翻译服务，参考 [开通服务](https:
 *        - 如需指定源语言，你需要在调用 `joinRoom` 接口进房时，通过 extraInfo 参数传入格式为`"语种英文名": "语种代号"` JSON 字符串，例如设置源语言为英文时，传入 `"source_language": "en"`。如未指定源语言，SDK 会将系统语种设定为源语言。如果你的系统语种不是中文、英文和日文，此时 SDK 会自动将中文设为源语言。
 *          - 识别模式下，你可以传入 [RTC 控制台](https:
 *          - 翻译模式下，你需要传入机器翻译规定的语种英文名和语种代号。翻译模式下支持的语言及对应的代号参看[翻译模式语言支持](https:
 * @list 字幕翻译服务
 */

- (int)startSubtitle:(ByteRTCSubtitleConfig *_Nonnull)subtitleConfig;
/**
 * 
 * @valid since 3.52
 * @type api
 * @brief 关闭字幕。 <br>
 *        调用该方法后，用户会收到 rtcRoom:onSubtitleStateChanged:errorCode:errorMessage:{@link #ByteRTCRoomDelegate#rtcRoom:onSubtitleStateChanged:errorCode:errorMessage:}  回调，通知字幕是否关闭。
 * @return
 *        -  0: 调用成功。
 *        - !0: 调用失败。
 * @list 字幕翻译服务
 */

- (int)stopSubtitle;

/** 
  * 
  * @type api
  * @valid since 3.60.
  * @brief 设置期望订阅的远端视频流类型。
  * @param streamId 目标要订阅的远端视频流 ID。
  * @param streamType 远端视频流类型，参看 ByteRTCVideoSimulcastStreamType{@link #ByteRTCVideoSimulcastStreamType}。
  * @return 方法调用结果： <br>
  *        - 0：成功；
  *        - <0：失败。
  * @note
  *        - 该方法仅在发布端调用 setVideoEncoderConfig:withParameters:{@link #ByteRTCEngine#setVideoEncoderConfig:withParameters:} 开启了发送多路视频流的情况下生效。
  *        - 若发布端开启了推送多路流功能，但订阅端不对流参数进行设置，则默认接受发送端设置的分辨率最大的一路视频流。该方法可在进房后调用。
  * @list 视频管理
  * @order 1
  */

- (int)setRemoteSimulcastStreamType:(NSString * _Nonnull) streamId streamType:(ByteRTCVideoSimulcastStreamType)streamType;

/**
 * 
 * @type api
 * @brief 设置期望订阅的远端视频流的参数。
 * @param streamId 期望配置订阅参数的远端视频流 ID。
 * @param remoteVideoConfig 期望配置的远端视频流参数，参看 ByteRTCRemoteVideoConfig{@link #ByteRTCRemoteVideoConfig}。
 * @return 方法调用结果： <br>
 *        + 0：成功。<br>
 *        + !0：失败。
 * @note 
 *        + 若使用 342 及以前版本的 SDK，调用该方法前请联系技术支持人员开启按需订阅功能。  <br>
 *        + 该方法仅在发布端调用 setLocalSimulcastMode:{@link #ByteRTCEngine#setLocalSimulcastMode:} 开启了发送多路视频流的情况下生效，此时订阅端将收到来自发布端与期望设置的参数最相近的一路流；否则订阅端只会收到一路参数为分辨率 640px × 360px、帧率 15fps 的视频流。  <br>
 *        + 若发布端开启了推送多路流功能，但订阅端不对流参数进行设置，则默认接受发送端设置的分辨率最大的一路视频流。  <br>
 *        + 该方法需在进房后调用。  <br>
 *        + SDK 会根据发布端和所有订阅端的设置灵活调整视频流的参数，具体调整策略详见[推送多路流](https:
 * @list 视频管理
 */

- (int)setRemoteVideoConfig:(NSString * _Nonnull) streamId remoteVideoConfig:(ByteRTCRemoteVideoConfig *_Nonnull) remoteVideoConfig;


/**
 * 
 * @type api
 * @valid since 3.60.
 * @brief 获取通话 ID。<br>
 *        该方法需要在加入房间后调用。当创建一个房间开启音视频通话后，系统会为该房间生成一个对应的通话 ID，标识此房间的通话。
 * @return 通话 ID。
 * @list 房间管理
 */

- (NSString * _Nonnull)getCallId;
@end
