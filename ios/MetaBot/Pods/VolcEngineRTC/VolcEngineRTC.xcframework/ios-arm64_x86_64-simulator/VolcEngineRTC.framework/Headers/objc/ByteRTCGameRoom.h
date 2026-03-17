/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room
*/

#import <CoreMedia/CMTime.h>
#import "rtc/ByteRTCDefines.h"
#import "rtc/ByteRTCRangeAudio.h"

@class ByteRTCGameRoom;
/**
 * 
 * @type callback
 * @brief 游戏房间事件回调接口 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 
 */

@protocol ByteRTCGameRoomDelegate <NSObject>
@optional
 /**
  * 
  * @hidden
  * @type callback
  * @brief 游戏房间状态改变回调，加入房间、异常退出房间、发生房间相关的警告或错误时会收到此回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param roomId 房间 ID。
  * @param uid 用户 ID。
  * @param state 房间状态码。 <br>
  *              - 0: 加入房间成功。
  *              - 1: 加入房间失败、异常退房、发生房间相关的警告或错误。
  *              - 2: 离开房间。
  * @param reason 房间状态发生变化的原因。参看 ByteRTCRoomStateChangeReason{@link #ByteRTCRoomStateChangeReason}。
  * @list 房间管理
  */
 
- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onRoomStateChangedWithReason:(NSString *_Nonnull)roomId
             withUid:(nonnull NSString *)uid
           state:(ByteRTCRoomState)state
           reason:(ByteRTCRoomStateChangeReason)reason;

 /**
  * 
  * @type callback
  * @brief 游戏房间状态改变回调，加入房间、异常退出房间、发生房间相关的警告或错误时会收到此回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param roomId 房间 ID。
  * @param uid 用户 ID。
  * @param state 房间状态码。 <br>
  *              - 0: 加入房间成功。
  *              - !0: 加入房间失败、异常退房、发生房间相关的警告或错误。具体原因参看 ByteRTCErrorCode{@link #ByteRTCErrorCode} 及 ByteRTCWarningCode{@link #ByteRTCWarningCode}。
  * @param extraInfo 额外信息，如 `{"elapsed":1187,"join_type":0}`。 <br>
  *                  `join_type`表示加入房间的类型，`0`为首次进房，`1`为重连进房。 <br>
  *                  `elapsed`表示加入房间耗时，即本地用户从调用 joinRoom:userInfo:{@link #ByteRTCGameRoom#joinRoom:userInfo:} 到加入房间成功所经历的时间间隔，单位为 ms。
  * @list 房间管理
  * @order 0
  */
 
- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onRoomStateChanged:(NSString *_Nonnull)roomId
             withUid:(nonnull NSString *)uid
           state:(NSInteger)state
           extraInfo:(NSString *_Nonnull)extraInfo;

/**
 * 
 * @type callback
 * @brief 音频流状态改变回调，发生流相关的警告或错误时会收到此回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param roomId 房间 ID。
 * @param uid 用户 ID。
 * @param state 流状态码，参看 ByteRTCErrorCode{@link #ByteRTCErrorCode} 及 ByteRTCWarningCode{@link #ByteRTCWarningCode}。
 * @param extraInfo 附加信息，默认为空。
 * @list 音视频传输
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onStreamStateChanged:(NSString *_Nonnull)roomId
            withUid:(nonnull NSString *)uid
          state:(NSInteger)state
          extraInfo:(NSString *_Nonnull)extraInfo;
/**
 * 
 * @type callback
 * @brief 离开游戏房间成功回调。 <br>
 *        用户调用 leaveRoom{@link #ByteRTCGameRoom#leaveRoom} 方法后，SDK 会停止所有的发布订阅流，并释放所有通话相关的音视频资源。SDK 完成所有的资源释放后通过此回调通知用户。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param stats 保留参数，目前为空。
 * @note
 *       - 用户调用 leaveRoom{@link #ByteRTCGameRoom#leaveRoom} 方法离开房间后，如果立即调用 destroy{@link #ByteRTCGameRoom#destroy} 销毁房间实例方法销毁 RTC 引擎，则将无法收到此回调事件。
 *       - 离开游戏房间结束通话后，如果 App 需要使用系统音视频设备，则建议在收到此回调后再初始化音视频设备，否则可能由于 SDK 占用了导致 App 初始化音视频设备失败。
 * @list 房间管理
 * @order 2
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onLeaveRoom:(ByteRTCRoomStats *_Nonnull)stats;

 /**
  * 
  * @deprecated since 3.41 and will be deleted in later version, use rtcRoom:onRoomStateChangedWithReason:withUid:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onRoomStateChangedWithReason:withUid:state:reason:}  and rtcRoom:onStreamStateChanged:withUid:state:extraInfo:{@link #ByteRTCGameRoomDelegate#rtcRoom:onStreamStateChanged:withUid:state:extraInfo:} instead.
  * @type callback
  * @brief 发生警告回调。 <br>
  *        SDK 运行时出现了警告。SDK 通常会自动恢复，警告信息可以忽略。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param warningCode 警告码，详见枚举类型 ByteRTCWarningCode{@link #ByteRTCWarningCode} 。
  * @list 房间管理
  */
 
- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onRoomWarning:(ByteRTCWarningCode)warningCode __deprecated_msg("Will be removed in new version");
/**
 * 
 * @type callback
 * @brief 发布端调用 setMultiDeviceAVSync:{@link #ByteRTCRoom#setMultiDeviceAVSync:} 后音视频同步状态发生改变时，会收到此回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param state 音视频同步状态，参看 ByteRTCAVSyncState{@link #ByteRTCAVSyncState}。
 * @list 音视频传输
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onAVSyncStateChange:(ByteRTCAVSyncState)state;
/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 视频发布状态改变回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param streamId 视频流 ID。
 * @param info 视频流信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}。
 * @param state 发布状态码，参看 ByteRTCPublishState{@link #ByteRTCPublishState}。
 * @param reason 本端视频流发布状态改变的具体原因，参看 ByteRTCPublishStateChangeReason{@link #ByteRTCPublishStateChangeReason}。 
 * @list 音视频传输
 * @order 0
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onVideoPublishStateChanged:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info
           state:(ByteRTCPublishState)state
           reason:(ByteRTCPublishStateChangeReason)reason;

/**
 * 
 * @type callback
 * @brief 音频发布状态改变回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param streamId 音频流 ID。
 * @param info 音频流信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}。
 * @param state 发布状态码，参看 ByteRTCPublishState{@link #ByteRTCPublishState}。
 * @param reason 本端音频流发布状态改变的具体原因，参看 ByteRTCPublishStateChangeReason{@link #ByteRTCPublishStateChangeReason}。 
 * @list 音视频传输
 * @order 0
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onAudioPublishStateChanged:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info
           state:(ByteRTCPublishState)state
           reason:(ByteRTCPublishStateChangeReason)reason;

/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 屏幕流视频发布状态改变回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param roomId 房间 ID。
 * @param uid 用户 ID。
 * @param state 发布状态码，参看 ByteRTCPublishState{@link #ByteRTCPublishState}。
 * @param reason 屏幕流视频发布状态改变的具体原因，参看 ByteRTCPublishStateChangeReason{@link #ByteRTCPublishStateChangeReason}。
 * @list 音视频传输
 * @order 0
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onScreenVideoPublishStateChanged:(NSString *_Nonnull)roomId
             userId:(nonnull NSString *)uid
           state:(ByteRTCPublishState)state
           reason:(ByteRTCPublishStateChangeReason)reason;


/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 屏幕流音频发布状态改变回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param roomId 房间 ID。
 * @param uid 用户 ID。
 * @param state 发布状态码，参看 ByteRTCPublishState{@link #ByteRTCPublishState}。
 * @param reason 屏幕流音频发布状态改变的具体原因，参看 ByteRTCPublishStateChangeReason{@link #ByteRTCPublishStateChangeReason}。
 * @list 音视频传输
 * @order 0
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onScreenAudioPublishStateChanged:(NSString *_Nonnull)roomId
             userId:(nonnull NSString *)uid
           state:(ByteRTCPublishState)state
           reason:(ByteRTCPublishStateChangeReason)reason;

/**
 * 
 * @type callback
 * @brief 视频订阅状态发生改变回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param streamId 视频流 ID。
 * @param info 视频流信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}。
 * @param state 订阅状态码，参看 ByteRTCSubscribeState{@link #ByteRTCSubscribeState}。
 * @param reason 视频订阅状态改变的具体原因，参看 ByteRTCSubscribeStateChangeReason{@link #ByteRTCSubscribeStateChangeReason}。
 * @list 音视频传输
 * @order 0
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onVideoSubscribeStateChanged:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info
           state:(ByteRTCSubscribeState)state
           reason:(ByteRTCSubscribeStateChangeReason)reason;

/**
 * 
 * @type callback
 * @brief 音频订阅状态发生改变回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param streamId 音频流 ID.
 * @param info 音频流信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}。
 * @param state 订阅状态码，参看 ByteRTCSubscribeState{@link #ByteRTCSubscribeState}。
 * @param reason 音频订阅状态改变的具体原因，参看 ByteRTCSubscribeStateChangeReason{@link #ByteRTCSubscribeStateChangeReason}。 
 * @list 音视频传输
 * @order 0
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onAudioSubscribeStateChanged:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info
           state:(ByteRTCSubscribeState)state
           reason:(ByteRTCSubscribeStateChangeReason)reason;

/**
 * 
 * @type callback
 * @brief 屏幕流视频订阅状态发生改变回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param roomId 房间 ID。
 * @param uid 用户 ID。
 * @param state 订阅状态码，参看 ByteRTCSubscribeState{@link #ByteRTCSubscribeState}。
 * @param reason 屏幕流视频订阅状态改变的具体原因，参看 ByteRTCSubscribeStateChangeReason{@link #ByteRTCSubscribeStateChangeReason}。 
 * @list 音视频传输
 * @order 0
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onScreenVideoSubscribeStateChanged:(NSString *_Nonnull)roomId
             userId:(nonnull NSString *)uid
           state:(ByteRTCSubscribeState)state
           reason:(ByteRTCSubscribeStateChangeReason)reason;

/**
 * 
 * @type callback
 * @brief 屏幕流音频订阅状态发生改变回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param roomId 房间 ID。
 * @param uid 用户 ID。
 * @param state 订阅状态码，参看 ByteRTCSubscribeState{@link #ByteRTCSubscribeState}。
 * @param reason 屏幕流音频订阅状态改变的具体原因，参看 ByteRTCSubscribeStateChangeReason{@link #ByteRTCSubscribeStateChangeReason}。
 * @list 音视频传输
 * @order 0
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onScreenAudioSubscribeStateChanged:(NSString *_Nonnull)roomId
             userId:(nonnull NSString *)uid
           state:(ByteRTCSubscribeState)state
           reason:(ByteRTCSubscribeStateChangeReason)reason;

/**
 * 
 * @type callback
 * @region 多房间
 * @brief 房间内通话统计信息回调。 <br>
 *        用户进房开始通话后，每 2s 收到一次本回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param stats 当前 ByteRTCGameRoom 统计数据，详见：ByteRTCRoomStats{@link #ByteRTCRoomStats}。
 * @list 房间管理
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onRoomStats:(ByteRTCRoomStats *_Nonnull)stats NS_SWIFT_NAME(rtcRoom(_:onRoomStats:));
/**
  * 
  * @hidden 仅内部使用
  * @type callback
  * @region 多房间
  * @valid since 3.60.
  * @brief 房间事件回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param roomId 房间 ID。
  * @param uid 用户 ID。
  * @param state 房间事件状态。详见 ByteRTCRoomEvent{@link #ByteRTCRoomEvent}。
  * @param info 房间封禁时，包含封禁时间。详见 ByteRTCRoomEventInfo{@link #ByteRTCRoomEventInfo}。
  * @list 房间管理
  * @order 0
  */
  
- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onRoomEvent:(NSString *_Nonnull)roomId uid:(NSString *_Nonnull)uid state:(ByteRTCRoomEvent)state info:(ByteRTCRoomEventInfo *_Nonnull)info;
/**
 * 
 * @type callback
 * @brief 本地流数据统计以及网络质量回调。 <br>
 *        本地用户发布流成功后，SDK 会周期性（2s）的通过此回调事件通知用户发布的流在此次统计周期内的质量统计信息。 <br>
 *        统计信息通过 ByteRTCLocalStreamStats{@link #ByteRTCLocalStreamStats} 类型的回调参数传递给用户，其中包括发送音视频比特率、发送帧率、编码帧率，网络质量等。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param streamId 流 ID。
 * @param info 流信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}。
 * @param stats 当前房间本地流数据统计。详见：ByteRTCLocalStreamStats{@link #ByteRTCLocalStreamStats}
 * @list 音视频传输
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onLocalStreamStats:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info stats:(ByteRTCLocalStreamStats *_Nonnull)stats NS_SWIFT_NAME(rtcRoom(_:onLocalStreamStats:));
/**
 * 
 * @type callback
 * @brief 本地订阅的远端音/视频流数据统计以及网络质量回调。 <br>
 *        本地用户订阅流成功后，SDK 会周期性（2s）的通过此回调事件通知用户订阅的流在此次统计周期内的质量统计信息，包括：发送音视频比特率、发送帧率、编码帧率，网络质量等。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param streamId 流 ID。
 * @param info 流信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}。
 * @param stats 当前房间本地流数据统计。 详见：ByteRTCRemoteStreamStats{@link #ByteRTCRemoteStreamStats}
 * @list 音视频传输
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onRemoteStreamStats:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info stats:(ByteRTCRemoteStreamStats *_Nonnull)stats NS_SWIFT_NAME(rtcRoom(_:onRemoteStreamStats:));
/**
 * 
 * @type callback
 * @brief 远端可见用户加入房间，或房内不可见用户切换为可见的回调。 <br>
 *        1. 远端可见用户断网后重新连入房间时，房间内其他用户将收到该事件。 <br>
 *        2. 新进房用户会收到进房前已在房内的可见用户的进房回调通知。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param userInfo 用户信息，参看 ByteRTCUserInfo{@link #ByteRTCUserInfo}。
 * @list 房间管理
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onUserJoined:(ByteRTCUserInfo *_Nonnull)userInfo;
/**
 * 
 * @type callback
 * @brief 远端用户离开游戏房间，或切至不可见时，本地用户会收到此事件
 * @param rtcRoom `ByteRTCGameRoom` 实例
 * @param uid 离开房间，或切至不可见的的远端用户 ID。
 * @param reason 用户离开房间的原因： <br>
 *              - 0: 远端用户调用 leaveRoom{@link #ByteRTCGameRoom#leaveRoom} 主动退出房间。
 *              - 1: 远端用户因 Token 过期或网络原因等掉线。详细信息请参看[连接状态提示](https:
 *              - 3: 服务端调用 OpenAPI 将远端用户踢出房间。
 * @list 房间管理
 */

- (void)rtcRoom:(ByteRTCGameRoom *_Nonnull)rtcRoom onUserLeave:(NSString *_Nonnull)uid reason:(ByteRTCUserOfflineReason)reason;
/**
 * 
 * @type callback
 * @brief 当 SDK 检测到 Token 的进房权限将在 30 秒内过期时，触发该回调。
 *        收到该回调后，你需调用 updateToken:{@link #ByteRTCGameRoom#updateToken:} 更新 Token 进房权限。
 * @param rtcRoom `ByteRTCGameRoom` 实例
 * @note 若 Token 进房权限过期且未及时更新： <br>
 *        - 用户此时尝试进房会收到 rtcRoom:onRoomStateChangedWithReason:withUid:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onRoomStateChangedWithReason:withUid:state:reason:}  回调，提示错误码为 `-1000` Token 无效；
 *        - 用户已在房间内则会被移出房间，本地用户会收到 rtcRoom:onRoomStateChangedWithReason:withUid:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onRoomStateChangedWithReason:withUid:state:reason:}  回调，提示错误码为 `-1009` Token 过期，同时远端用户会收到 rtcRoom:onUserLeave:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserLeave:reason:} 回调，提示原因为 `1` Token 进房权限过期。
 * @list 引擎管理
 */
 
- (void)onTokenWillExpire:(ByteRTCGameRoom *_Nonnull)rtcRoom;
/**
 * 
 * @type callback
 * @brief Token 发布权限过期前 30 秒将触发该回调。 <br>
 *        收到该回调后，你需调用 updateToken:{@link #ByteRTCGameRoom#updateToken:} 更新 Token 发布权限。
 * @param rtcRoom `ByteRTCGameRoom` 实例
 * @note Token 发布权限过期后：
     *        - 已发布流或尝试发布流时，本端会收到 rtcRoom:onVideoPublishStateChanged:info:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onVideoPublishStateChanged:info:state:reason:}、rtcRoom:onAudioPublishStateChanged:info:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onAudioPublishStateChanged:info:state:reason:}、rtcRoom:onScreenVideoPublishStateChanged:userId:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onScreenVideoPublishStateChanged:userId:state:reason:}、rtcRoom:onScreenAudioPublishStateChanged:userId:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onScreenAudioPublishStateChanged:userId:state:reason:} 回调，提示`kPublishStateChangeReasonNoPublishPermission`，没有发布权限。
     *        - 发布中的流将停止发布。远端用户会收到 rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}、rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:}、rtcRoom:onUserPublishScreenVideo:uid:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishScreenVideo:uid:isPublish:} 或 rtcRoom:onUserPublishScreenAudio:uid:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishScreenAudio:uid:isPublish:} 回调，提示该流已停止发布。
 * @list 引擎管理
 * @order 3
 */

- (void)onPublishPrivilegeTokenWillExpire:( ByteRTCGameRoom *_Nonnull)rtcRoom;
/**
 * 
 * @type callback
 * @brief Token 订阅权限过期前 30 秒将触发该回调。 <br>
 *        收到该回调后，你需调用 updateToken:{@link #ByteRTCGameRoom#updateToken:} 更新 Token 订阅权限有效期。
 * @param rtcRoom `ByteRTCGameRoom` 实例
 * @note 若收到该回调后未及时更新 Token，Token 订阅权限过期后，尝试新订阅流会失败，已订阅的流会取消订阅，并且会收到 rtcRoom:onStreamStateChanged:withUid:state:extraInfo:{@link #ByteRTCGameRoomDelegate#rtcRoom:onStreamStateChanged:withUid:state:extraInfo:} 回调，提示错误码为 `-1003` 没有订阅权限。
 * @list 引擎管理
 */

- (void)onSubscribePrivilegeTokenWillExpire:( ByteRTCGameRoom *_Nonnull)rtcRoom;
 /**
  *  
  * @deprecated since 3.36 and will be deleted in later version, use rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:}, rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}, rtcRoom:onUserPublishScreenVideo:uid:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishScreenVideo:uid:isPublish:}, and rtcRoom:onUserPublishScreenAudio:uid:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishScreenAudio:uid:isPublish:} instead.
  * @type callback
  * @brief 以下情况会触发此回调： <br>
  *        - 房间内的用户发布新的音视频流时，房间内的其他用户会收到此回调通知。
  *        - 房间内的用户原音视频流被移出后，又重新发布音视频流时，房间内的其他用户会收到此回调通知。
  *        - 用户刚加入房间时，会收到此回调，包含房间中所有已发布的流。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param stream 流属性，参看 ByteRTCStream{@link #ByteRTCStream} 。
  * @list 房间管理
  */
 
- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onStreamAdd:(id<ByteRTCStream> _Nonnull)stream __deprecated_msg("Will be removed in new version");
 /**
  * 
  * @hidden
  * @deprecated since 3.36 and will be deleted in later version, use rtcRoom:onUserPublishStreamAudio:info:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishStreamAudio:info:isPublish:}, rtcRoom:onUserPublishStreamVideo:info:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishStreamVideo:info:isPublish:}, rtcRoom:onUserPublishScreenVideo:uid:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishScreenVideo:uid:isPublish:}, and rtcRoom:onUserPublishScreenAudio:uid:isPublish:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserPublishScreenAudio:uid:isPublish:} instead.
  * @type callback
  * @brief 房间内的远端用户停止发布音视频流时，本地用户会收到此回调通知。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param uid 远端流来源的用户 ID 。
  * @param stream 流的属性，参看 ByteRTCStream{@link #ByteRTCStream}。
  * @param reason 远端流移除的原因，参看 ByteRTCStreamRemoveReason{@link #ByteRTCStreamRemoveReason} 。
  * @list 房间管理
  */
 
- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onStreamRemove:(NSString *_Nonnull)uid
              stream:(id<ByteRTCStream> _Nonnull)stream
              reason:(ByteRTCStreamRemoveReason)reason
              __deprecated_msg("Will be removed in new version");

/**
 * 
 * @hidden for internal use only
 * @type callback
 * @region 多房间
 * @brief 当发布流成功的时候回调该事件
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param userId 用户 ID
 * @param isScreen 该流是否是屏幕流 <br>
 *
 */


- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onStreamPublishSuccess:(NSString *_Nonnull)userId isScreen:(BOOL)isScreen;

/**
  * 
  * @type callback
  * @valid since 3.60.
  * @brief 发布端调用 setMultiDeviceAVSync:{@link #ByteRTCRoom#setMultiDeviceAVSync:} 后音视频同步状态发生错误时，会收到此回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param roomId 房间 ID。
  * @param userId 用户 ID。
  * @param eventCode 音视频同步状态错误，参看 ByteRTCAVSyncEvent{@link #ByteRTCAVSyncEvent}。
  * @list 音视频传输
  * @order 1
  */
 
- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onAVSyncEvent:(NSString *_Nonnull)roomId userId:(NSString *_Nonnull)userId eventCode:(ByteRTCAVSyncEvent)eventCode;

/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 房间内远端用户发布或取消发布视频流的回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param streamId 流 ID。
 * @param info 流信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}。
 * @param isPublish 流是否发布。
 *         + `true`：已发布。
 *         + `false`：已取消发布。
 * @list 音视频传输
 * @order 2
 */
 
- (void)rtcRoom:(ByteRTCGameRoom* _Nonnull)rtcRoom onUserPublishStreamVideo:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info
isPublish:(BOOL)isPublish;

/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 房间内远端音频流发布回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param streamId 流 ID。
 * @param info 流信息，参看 ByteRTCStreamInfo{@link #ByteRTCStreamInfo}。
 * @param isPublish 流是否发布。
 *         + `true`：已发布。
 *         + `false`：已取消发布。
 * @list 音视频传输
 * @order 2
 */
 
- (void)rtcRoom:(ByteRTCGameRoom* _Nonnull)rtcRoom onUserPublishStreamAudio:(NSString *_Nonnull)streamId
info:(ByteRTCStreamInfo* _Nonnull)info
isPublish:(BOOL)isPublish;

/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 房间内远端屏幕共享的视频流的回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param roomId 房间 ID。
 * @param uid 远端流发布用户的用户 ID。
 * @param isPublish 为 true 代表已发布，为 false 代表已取消发布。
 * @list 音视频传输
 * @order 2
 */
 
- (void)rtcRoom:(ByteRTCGameRoom* _Nonnull)rtcRoom onUserPublishScreenVideo:(NSString* _Nonnull)roomId uid:(NSString* _Nonnull)uid isPublish:(BOOL)isPublish;

/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 房间内远端屏幕共享的音频流的回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param roomId 房间 ID。
 * @param uid 远端流发布用户的用户 ID。
 * @param isPublish 为 true 代表已发布，为 false 代表已取消发布。
 * @list 音视频传输
 * @order 2
 */
 
- (void)rtcRoom:(ByteRTCGameRoom* _Nonnull)rtcRoom onUserPublishScreenAudio:(NSString* _Nonnull)roomId uid:(NSString* _Nonnull)uid isPublish:(BOOL)isPublish;
 /**
  * 
  * @type callback
  * @brief 广播文本消息回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param uid 消息发送者 ID。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onRoomMessageReceived:(NSString *_Nonnull)uid
        message:(NSString *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到房间内广播二进制消息的回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param uid 消息发送者 ID 。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onRoomBinaryMessageReceived:(NSString *_Nonnull)uid
        message:(NSData *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到来自房间中其他用户发来的点对点文本消息时，会收到此回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onUserMessageReceived:(NSString *_Nonnull)uid
               message:(NSString *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到来自房间中其他用户发来的点对点二进制消息时，会收到此回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */

- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onUserBinaryMessageReceived:(NSString *_Nonnull)uid
               message:(NSData *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 向房间内单个用户发送文本或二进制消息后（P2P），消息发送方会收到该消息发送结果回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param msgid 本条消息的 ID。
  * @param error 消息发送结果，详见 ByteRTCUserMessageSendResult{@link #ByteRTCUserMessageSendResult}
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onUserMessageSendResult:(NSInteger)msgid error:(ByteRTCUserMessageSendResult)error;
 /**
  * 
  * @type callback
  * @brief 向房间内群发文本或二进制消息后，消息发送方会收到该消息发送结果回调。
  * @param rtcRoom ByteRTCGameRoom 实例。
  * @param msgid 本条消息的 ID。
  * @param error 消息发送结果，详见 ByteRTCRoomMessageSendResult{@link #ByteRTCRoomMessageSendResult}
  * @list 消息
  */
 
- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onRoomMessageSendResult:(NSInteger)msgid error:(ByteRTCRoomMessageSendResult)error;
/**
 * 
 * @valid since 3.52.
 * @type callback
 * @brief 发送附加信息结果回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param taskId 本次调用的任务编号。
 * @param result 房间附加信息设置结果，详见 ByteRTCSetRoomExtraInfoResult{@link #ByteRTCSetRoomExtraInfoResult}
 * @list 房间管理
 */

- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onSetRoomExtraInfoResult:(NSInteger)taskId result:(ByteRTCSetRoomExtraInfoResult)result;
/**
 * 
 * @valid since 3.52.
 * @type callback
 * @brief 接收到房间附加信息的回调。<br>
 *        另外用户加入房间成功后会收到这个房间全量附加信息
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param key 附加信息的键值
 * @param value 附加信息的内容
 * @param lastUpdateUserId 最后更新这条附加信息的用户编号
 * @param lastUpdateTimeMs 最后更新这条附加信息的 Unix 时间，时间精度是毫秒
 * @list 房间管理
 */

- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onRoomExtraInfoUpdate:(NSString *_Nonnull)key value:(NSString *_Nonnull)value lastUpdateUserId:(NSString *_Nonnull)lastUpdateUserId lastUpdateTimeMs:(NSInteger)lastUpdateTimeMs;
/**
 * 
 * @valid since 3.54
 * @type callback
 * @brief 设置用户可见性的回调。
 * @param rtcRoom `ByteRTCGameRoom` 实例
 * @param currentUserVisibility 当前用户的可见性。 <br>
 *        - YES: 可见，用户可以在房间内发布音视频流，房间中的其他用户将收到用户的行为通知，例如进房、开启视频采集和退房。
 *        - NO: 不可见，用户不可以在房间内发布音视频流，房间中的其他用户不会收到用户的行为通知，例如进房、开启视频采集和退房。
 * @param errorCode 设置用户可见性错误码，参看 ByteRTCUserVisibilityChangeError{@link #ByteRTCUserVisibilityChangeError}。
 * @list 房间管理
 */

- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onUserVisibilityChanged:(BOOL)currentUserVisibility errorCode:(ByteRTCUserVisibilityChangeError)errorCode;
/**
 * 
 * @type callback
 * @brief 通过调用服务端 BanUserStream/UnbanUserStream 方法禁用/解禁指定房间内指定用户视频流的发送时，触发此回调。
 * @param rtcRoom `ByteRTCGameRoom` 实例
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

- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onVideoStreamBanned:(NSString *_Nonnull)uid isBanned:(BOOL)banned;
/**
 * 
 * @type callback
 * @brief 通过调用服务端 BanUserStream/UnbanUserStream 方法禁用/解禁指定房间内指定用户音频流的发送时，触发此回调。
 * @param rtcRoom `ByteRTCGameRoom` 实例
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

- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onAudioStreamBanned:(NSString *_Nonnull)uid isBanned:(BOOL)banned;

#pragma mark ForwardStream related callback
    /**
     * 
     * @type callback
     * @brief 跨房间媒体流转发状态和错误回调
     * @param rtcRoom ByteRTCGameRoom 实例。
     * @param infos 跨房间媒体流转发目标房间信息数组，详见 ByteRTCForwardStreamStateInfo{@link #ByteRTCForwardStreamStateInfo}
     * @list 音视频传输
     */
    
- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onForwardStreamStateChanged:(NSArray<ByteRTCForwardStreamStateInfo *> * _Nonnull)infos;
    /**
     * 
     * @type callback
     * @brief 跨房间媒体流转发事件回调
     * @param rtcRoom ByteRTCGameRoom 实例。
     * @param infos 跨房间媒体流转发目标房间事件数组，详见 ByteRTCForwardStreamEventInfo{@link #ByteRTCForwardStreamEventInfo}
     * @list 音视频传输
     */
    
- (void)rtcRoom:( ByteRTCGameRoom * _Nonnull)rtcRoom onForwardStreamEvent:(NSArray<ByteRTCForwardStreamEventInfo *> * _Nonnull)infos;
/**
 * 
 * @type callback
 * @brief 加入房间并发布或订阅流后， 以每 2 秒一次的频率，报告本地用户和已订阅的远端用户的上下行网络质量信息。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param localQuality 本端网络质量，详见 ByteRTCNetworkQualityStats{@link #ByteRTCNetworkQualityStats}。
 * @param remoteQualities 已订阅用户的网络质量，详见 ByteRTCNetworkQualityStats{@link #ByteRTCNetworkQualityStats}。
 * @note 更多通话中的监测接口，详见[通话中质量监测](https:
 * @list 音视频传输
 */

- (void)rtcRoom:(ByteRTCGameRoom * _Nonnull)rtcRoom onNetworkQuality:(ByteRTCNetworkQualityStats *_Nonnull)localQuality remoteQualities:(NSArray<ByteRTCNetworkQualityStats*> *_Nonnull)remoteQualities;
/**
 * 
 * @type callback
 * @brief 字幕状态发生改变回调。 <br>
 *         当用户调用 startSubtitle:{@link #ByteRTCRoom#startSubtitle:} 和 stopSubtitle{@link #ByteRTCRoom#stopSubtitle} 使字幕状态发生改变或出现错误时，触发该回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param state 字幕状态。参看 ByteRTCSubtitleState{@link #ByteRTCSubtitleState}。
 * @param errorCode 字幕任务错误码。参看 ByteRTCSubtitleErrorCode{@link #ByteRTCSubtitleErrorCode}。
 * @param errorMessage 第三方服务出现的错误。当因第三方服务出现错误，导致字幕状态改变时，用户可通过此参数获取具体的错误信息。如果不是因为第三方服务导致字幕状态改变，该字段为空。
 * @list 字幕翻译服务
 */

- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onSubtitleStateChanged:(ByteRTCSubtitleState)state
                                                             errorCode:(ByteRTCSubtitleErrorCode)errorCode
                                                          errorMessage:(NSString *_Nonnull)errorMessage;
/**
 * 
 * @type callback
 * @brief 字幕相关内容回调。
 * @param rtcRoom ByteRTCGameRoom 实例。
 * @param subtitles 字幕消息内容。参看 ByteRTCSubtitleMessage{@link #ByteRTCSubtitleMessage}。
 * @list 字幕翻译服务
 */

- (void)rtcRoom:( ByteRTCGameRoom *_Nonnull)rtcRoom onSubtitleMessageReceived:(NSArray<ByteRTCSubtitleMessage*> * _Nonnull)subtitles;
@end
/**
 * 
 * @type api
 * @list 房间管理
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCGameRoom : NSObject
/**
 * 
 * @type callback
 * @list 房间管理
 */

@property(nonatomic, weak) id<ByteRTCGameRoomDelegate> _Nullable delegate;

#pragma mark - Core Methods
/**
 * 
 * @type api
 * @brief 退出并销毁调用 createGameRoom:roomConfig:{@link #ByteRTCEngine#createGameRoom:roomConfig:} 所创建的房间实例。
 * @list 房间管理
 */

- (void)destroy;
/**
 * 
 * @type api
 * @brief 通过设置 ByteRTCGameRoom{@link #ByteRTCGameRoom} 对象的事件句柄，监听此对象对应的回调事件。
 * @param roomDelegate 参见 ByteRTCRoomDelegate{@link #ByteRTCRoomDelegate}。
 * @return
 *        - 0: 调用成功。
 *        - < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @list 房间管理
 */


- (int)setRTCGameRoomDelegate:(id<ByteRTCGameRoomDelegate> _Nullable)roomDelegate;
/**
 * 
 * @type api
 * @brief 加入游戏房间。 <br>
 *        调用 createGameRoom:roomConfig:{@link #ByteRTCEngine#createGameRoom:roomConfig:} 创建房间后，调用此方法加入房间，同房间内其他用户进行音频通话。
 * @param token 动态密钥，用于对登录用户进行鉴权验证。 <br>
 *        进入房间需要携带 Token。测试时可使用控制台生成临时 Token，正式上线需要使用密钥 SDK 在您的服务端生成并下发 Token。Token 有效期及生成方式参看[使用 Token 完成鉴权](70121)。 <br>
 *       - 使用不同 App ID 的 App 是不能互通的。
 *       - 请务必保证生成 Token 使用的 App ID 和创建引擎时使用的 App ID 相同，否则会导致加入房间失败。
 * @param userInfo 用户信息，参看 ByteRTCUserInfo{@link #ByteRTCUserInfo}。
 * @return
 *        -  0: 成功。触发以下回调：
 *          - 本端收到房间状态通知 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCGameRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调。
 *          - 如果本端用户为可见用户，房间内其他用户收到 rtcRoom:onUserJoined:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserJoined:} 回调通知。
 *        - -1：room_id / user_info.uid 包含了无效的参数。
 *        - -2：已经在房间内。接口调用成功后，只要收到返回值为 0 ，且未调用 leaveRoom:{@link #ByteRTCGameRoom#leaveRoom} 成功，则再次调用进房接口时，无论填写的房间 ID 和用户 ID 是否重复，均触发此返回值。
 *        调用失败时，具体失败原因会通过 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCGameRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调告知。
 * @note
 *       - 同一个 App ID 的同一个房间内，每个用户的用户 ID 必须是唯一的。如果两个用户的用户 ID 相同，则后进房的用户会将先进房的用户踢出房间，并且先进房的用户会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCGameRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调通知，错误类型详见 ByteRTCErrorCode{@link #ByteRTCErrorCode} 中的 ByteRTCErrorCodeDuplicateLogin。
 *       - 用户加入房间成功后，在本地网络状况不佳的情况下，SDK 可能会与服务器失去连接，并触发 rtcEngine:onConnectionStateChanged:{@link #ByteRTCEngineDelegate#rtcEngine:onConnectionStateChanged:} 回调。此时 SDK 会自动重试，直到成功重连。重连成功后，本地会收到 rtcRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCGameRoomDelegate#rtcRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调通知。
 * @list 房间管理
 */

- (int)joinRoom:(NSString *_Nullable)token userInfo:(ByteRTCUserInfo *_Nonnull)userInfo;
/**
 * 
 * @type api
 * @brief 离开游戏房间。 <br>
 *        调用此方法结束通话过程，并释放所有通话相关的资源。
 * @return
 *        - 0: 调用成功。如果用户是房间内可见用户，触发以下回调：
 *            - 远端用户收到 rtcRoom:onUserLeave:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onUserLeave:reason:} 回调通知。
 *            - 正在发布的流会被取消发布。远端用户收到 rtcRoom:onVideoPublishStateChanged:info:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onVideoPublishStateChanged:info:state:reason:}、rtcRoom:onAudioPublishStateChanged:info:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onAudioPublishStateChanged:info:state:reason:}、rtcRoom:onScreenVideoPublishStateChanged:userId:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onScreenVideoPublishStateChanged:userId:state:reason:} 和/或 rtcRoom:onScreenAudioPublishStateChanged:userId:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onScreenAudioPublishStateChanged:userId:state:reason:} 回调通知。
 *        - < 0: 调用失败，参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明。
 * @note
 *       - 加入房间后，必须调用此方法结束通话，否则无法开始下一次通话。
 *       - 此方法是异步操作，调用返回时并没有真正退出房间。真正退出房间后，必须调用 joinRoom:userInfo:{@link #ByteRTCGameRoom#joinRoom:userInfo:} 再次加入房间。
 * @list 房间管理
 */

- (int)leaveRoom NS_SWIFT_NAME(leaveRoom());
/**
 * 
 * @type api
 * @brief 更新游戏房间 Token。 <br>
 *        收到 onTokenWillExpire:{@link #ByteRTCGameRoomDelegate#onTokenWillExpire:}，onPublishPrivilegeTokenWillExpire:{@link #ByteRTCGameRoomDelegate#onPublishPrivilegeTokenWillExpire:}，或 onSubscribePrivilegeTokenWillExpire:{@link #ByteRTCGameRoomDelegate#onSubscribePrivilegeTokenWillExpire:} 时，你必须重新获取 Token，并调用此方法更新 Token，以保证通话的正常进行。
 * @param token 重新获取的有效 Token。 <br>
 *        如果 Token 无效，你会收到 rtcRoom:onRoomStateChangedWithReason:withUid:state:reason:{@link #ByteRTCGameRoomDelegate#rtcRoom:onRoomStateChangedWithReason:withUid:state:reason:} ，错误码是 `-1010`。
 * @return
 *        - 0：成功；
 *        - !0：失败。
 * @note 请勿同时调用 updateToken:{@link #ByteRTCGameRoom#updateToken:} 和 joinRoom:userInfo:{@link #ByteRTCGameRoom#joinRoom:userInfo:} 重新加入房间。
 * @list 房间管理
 */

- (int)updateToken:(NSString *_Nullable)token;
/**
 * 
 * @type api
 * @brief 获取游戏房间的范围语音接口实例。
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
 * @brief 加入游戏房间后，调用该接口开启或关闭麦克风。同游戏房间其他用户会收到回调 rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:{@link #ByteRTCEngineDelegate#rtcEngine:onAudioDeviceStateChanged:device_type:device_state:device_error:}。
 * @param enable 是否开启麦克风：<br>
 *               - true：开启麦克风，采集并发布音频流。
 *               - false：默认设置。关闭麦克风，停止采集和发布音频流。
 * @return
 *        - 0：接口调用成功。
 *        - -3：接口调用失败。没有加入房间。
 * @note 不可与 enableAudioSend:{@link #ByteRTCGameRoom#enableAudioSend:} 同时调用。
 * @list 音频管理
 */

- (int)enableMicrophone:(BOOL)enable;
/**
 * 
 * @type api
 * @brief 加入游戏房间后，开启或关闭扬声器。
 * @param enable 是否开启声器：<br>
 *                - true：开启扬声器。
 *                - false：默认设置。关闭扬声器。
 * @return
 *         - 0：接口调用成功。
 *         - -3：接口调用失败。没有加入房间。
 * @list 音频管理
 */

- (int)enableSpeakerphone:(BOOL)enable;
/**
 * 
 * @type api
 * @brief 开始或停止发送音频流。调用此接口不影响音频采集。同游戏房间其他用户会收到相应的回调。
 * @param enable 是否发送音频流：<br>
 *               - true：发布音频流。
 *               - false：默认设置。停止发布音频流（不会关闭麦克风），即静音。
 * @return
 *        - 0：接口调用成功。
 *        - -3：接口调用失败，未加入房间。
 * @note 不可与 enableMicrophone:{@link #ByteRTCGameRoom#enableMicrophone:} 同时调用。
 * @list 音频管理
 */

- (int)enableAudioSend:(BOOL)enable;
/**
 * 
 * @type api
 * @brief 是否订阅指定用户的音频流。
 * @param userId 用户 ID，最大长度为128字节的非空字符串。支持的字符集范围为:<br>
 *            1. 26个大写字母 A ~ Z<br>
 *            2. 26个小写字母 a ~ z<br>
 *            3. 10个数字 0 ~ 9<br>
 *            4. 下划线"_", at符"@", 减号"-"
 * @param enable 是否订阅音频流：<br>
 *        true：订阅指定用户的音频流。
 *        false：默认设置。不订阅指定用户的音频流。
 * @return
 *        - 0：接口调用成功
 *        - -2：传入的用户 ID 为空字符串。
 * @list 音频管理
 */

- (int)enableAudioReceive:(NSString* _Nullable)userId enable:(BOOL)enable;
/**
 * 
 * @type api
 * @brief 调节某个游戏房间内所有远端用户的音频播放音量。
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
@end
