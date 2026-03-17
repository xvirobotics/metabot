/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief ByteRTCRTSRoom
*/

#import <CoreMedia/CMTime.h>
#import "rtc/ByteRTCRtsDefines.h"
#import "rtc/ByteRTCDefines.h"

@class ByteRTCRTSRoom;

/**
 * 
 * @type callback
 * @brief RTS 房间事件回调接口
 */

@protocol ByteRTCRTSRoomDelegate <NSObject>
@optional

 /**
  * 
  * @type callback
  * @list 房间管理
  * @brief RTS 房间状态改变回调，加入房间、异常退出房间、发生房间相关的警告或错误时会收到此回调。
  * @param rtsRoom RTS 房间实例。
  * @param roomId 房间 ID。
  * @param uid 用户 ID。
  * @param state 房间状态码。  <br>
  *              + 0: 加入房间成功。  <br>
  *              + !0: 加入房间失败、异常退房、发生房间相关的警告或错误。
  * @param extraInfo 额外信息，如 `{"elapsed":1187,"join_type":0}`。<br>
  *                  - `join_type` 表示加入房间的类型，`0`为首次进房，`1`为重连进房。<br>
  *                  - `elapsed` 表示加入房间耗时，即本地用户从调用 joinRTSRoom:userInfo:{@link #ByteRTCRTSRoom#joinRTSRoom:userInfo:} 到加入 RTS 房间成功所经历的时间间隔，单位为 ms。
  */
 
- (void)rtsRoom:(ByteRTCRTSRoom * _Nonnull)rtsRoom onRoomStateChanged:(NSString * _Nonnull)roomId
        withUid:(nonnull NSString *)uid state:(NSInteger)state
       extraInfo:(nonnull NSString *)extraInfo;

/**
 * 
 * @type callback
 * @list 房间管理
 * @brief 离开 RTS 房间成功回调。  <br>
 *        用户调用 leaveRoom{@link #ByteRTCRTSRoom#leaveRoom} 方法后，SDK 会停止房间内消息的收发，并在释放所有相关资源后，通过此回调通知用户离开房间成功。  <br>
 * @param rtsRoom RTSRoom 对象。  <br>
 * @param stats 保留参数，目前为空。
 * @note
 *       + 用户调用 leaveRoom{@link #ByteRTCRTSRoom#leaveRoom} 方法离开房间后，如果立即调用 destroy{@link #ByteRTCRTSRoom#destroy} 销毁房间实例，则将无法收到此回调事件。  <br>
 *       + 离开房间结束通话后，如果 App 需要使用系统音视频设备，则建议在收到此回调后再初始化音视频设备，否则可能由于 SDK 占用了导致 App 初始化音视频设备失败。  <br>
 */

- (void)rtsRoom:( ByteRTCRTSRoom *_Nonnull)rtsRoom onLeaveRoom:(ByteRTCRoomStats *_Nonnull)stats;

/**
 * 
 * @type callback
 * @list 房间管理
 * @brief 远端用户首次进房，或断网后重新连入房间时，房间内其他用户将收到该事件。
 * @param rtsRoom ByteRTCRTSRoom 对象。  <br>
 * @param userInfo 用户信息，参看 ByteRTCUserInfo{@link #ByteRTCUserInfo}。
 */

- (void)rtsRoom:(ByteRTCRTSRoom * _Nonnull)rtsRoom onUserJoined:(ByteRTCUserInfo *_Nonnull)userInfo;
/**
 * 
 * @type callback
 * @brief 远端用户离开 RTS 房间时，本地用户会收到此事件。
 * @param rtsRoom ByteRTCRTSRoom 对象。  <br>
 * @param uid 离开房间的远端用户 ID 。  <br>
 * @param reason 用户离开房间的原因，参看 ByteRTCUserOfflineReason{@link #ByteRTCUserOfflineReason}。
 * @list 房间管理
 */

- (void)rtsRoom:(ByteRTCRTSRoom * _Nonnull)rtsRoom onUserLeave:(NSString * _Nonnull)uid
         reason:(ByteRTCUserOfflineReason)reason;

#pragma mark - Message Delegate Methods
 /**
  * 
  * @type callback
  * @brief 收到房间中调用 sendRoomMessage:{@link #ByteRTCRTSRoom#sendRoomMessage:} 发送的广播文本消息时，收到此回调。
  * @param rtsRoom ByteRTCRTSRoom 对象
  * @param uid 消息发送者 ID 。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtsRoom:(ByteRTCRTSRoom * _Nonnull)rtsRoom onRoomMessageReceived:(NSString * _Nonnull)uid
        message:(NSString * _Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到房间中调用 sendRoomBinaryMessage:{@link #ByteRTCRTSRoom#sendRoomBinaryMessage:} 发送的广播二进制消息时，收到此回调。
  * @param rtsRoom ByteRTCRTSRoom 对象
  * @param uid 消息发送者 ID 。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */
 
- (void)rtsRoom:(ByteRTCRTSRoom * _Nonnull)rtsRoom onRoomBinaryMessageReceived:(NSString * _Nonnull)uid
        message:(NSData * _Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 向房间内所有用户群发文本或二进制消息后，消息发送方会收到该消息发送结果回调。
  * @param rtsRoom ByteRTCRTSRoom 对象。
  * @param msgid 本条消息的 ID。
  * @param error 消息发送结果，详见 ByteRTCRoomMessageSendResult{@link #ByteRTCRoomMessageSendResult}
  * @note  <br>
  *        + 你应调用 sendRoomMessage:{@link #ByteRTCRTSRoom#sendRoomMessage:} 向房间内群发文本消息 <br>
  *        + 你应调用 sendRoomBinaryMessage:{@link #ByteRTCRTSRoom#sendRoomBinaryMessage:} 向房间内群发二进制消息
  * @list 消息
  */
 
- (void)rtsRoom:(ByteRTCRTSRoom * _Nonnull)rtsRoom onRoomMessageSendResult:(NSInteger)msgid
          error:(ByteRTCRoomMessageSendResult)error;

 /**
  * 
  * @type callback
  * @brief 收到来自房间中其他用户通过 sendUserMessage:message:config:{@link #ByteRTCRTSRoom#sendUserMessage:message:config:} 发来的点对点文本消息时，会收到此回调。
  * @param rtsRoom ByteRTCRoom 对象
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtsRoom:( ByteRTCRTSRoom *_Nonnull)rtsRoom onUserMessageReceived:(NSString *_Nonnull)uid
               message:(NSString *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到来自房间中其他用户通过 sendUserBinaryMessage:message:config:{@link #ByteRTCRTSRoom#sendUserBinaryMessage:message:config:} 发来的点对点二进制消息时，会收到此回调。
  * @param rtsRoom ByteRTCRoom 对象
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */

- (void)rtsRoom:( ByteRTCRTSRoom *_Nonnull)rtsRoom onUserBinaryMessageReceived:(NSString *_Nonnull)uid
               message:(NSData *_Nonnull)message;
/**
  * 
  * @type callback
  * @brief 收到房间中调用 sendRoomMessage:{@link #ByteRTCRTSRoom#sendRoomMessage:} 发送的广播文本消息时，收到此回调。
  * @param rtsRoom ByteRTCRoom 对象
  * @param msgid 消息编号。
  * @param uid 消息发送者 ID 。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtsRoom:( ByteRTCRTSRoom *_Nonnull)rtsRoom onRoomMessageReceived:(NSInteger)msgid uid:(NSString *_Nonnull)uid
        message:(NSString *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到房间内广播二进制消息的回调。 <br>
  *        房间内其他用户调用 sendRoomBinaryMessage:{@link #ByteRTCRTSRoom#sendRoomBinaryMessage:} 发送的广播二进制消息时，收到此回调。
  * @param rtsRoom ByteRTCRoom 对象
  * @param msgid 消息编号。
  * @param uid 消息发送者 ID 。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */
 
- (void)rtsRoom:( ByteRTCRTSRoom *_Nonnull)rtsRoom onRoomBinaryMessageReceived:(NSInteger)msgid uid:(NSString *_Nonnull)uid
        message:(NSData *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到来自房间中其他用户通过 sendUserMessage:message:config:{@link #ByteRTCRTSRoom#sendUserMessage:message:config:} 发来的点对点文本消息时，会收到此回调。
  * @param rtsRoom ByteRTCRoom 对象
  * @param msgid 消息编号。
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的文本消息内容。
  * @list 消息
  */
 
- (void)rtsRoom:( ByteRTCRTSRoom *_Nonnull)rtsRoom onUserMessageReceived:(NSInteger)msgid uid:(NSString *_Nonnull)uid
               message:(NSString *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 收到来自房间中其他用户通过 sendUserBinaryMessage:message:config:{@link #ByteRTCRTSRoom#sendUserBinaryMessage:message:config:} 发来的点对点二进制消息时，会收到此回调。
  * @param rtsRoom ByteRTCRoom 对象
  * @param msgid 消息编号.
  * @param uid 消息发送者的用户 ID。
  * @param message 收到的二进制消息内容。
  * @list 消息
  */

- (void)rtsRoom:( ByteRTCRTSRoom *_Nonnull)rtsRoom onUserBinaryMessageReceived:(NSInteger)msgid uid:(NSString *_Nonnull)uid
               message:(NSData *_Nonnull)message;
 /**
  * 
  * @type callback
  * @brief 向房间内单个用户发送文本或二进制消息后（P2P），消息发送方会收到该消息发送结果回调。
  * @param rtsRoom ByteRTCRoom 对象。
  * @param msgid 本条消息的 ID。
  * @param error 消息发送结果，详见 ByteRTCUserMessageSendResult{@link #ByteRTCUserMessageSendResult}
  * @note
  *        - 你应调用 sendUserMessage:message:config:{@link #ByteRTCRTSRoom#sendUserMessage:message:config:} 向房间内单个用户发送文本消息
  *        - 你应调用 sendUserBinaryMessage:message:config:{@link #ByteRTCRTSRoom#sendUserBinaryMessage:message:config:} 向房间内单个用户发送二进制消息
  * @list 消息
  */
 
- (void)rtsRoom:( ByteRTCRTSRoom *_Nonnull)rtsRoom onUserMessageSendResult:(NSInteger)msgid error:(ByteRTCUserMessageSendResult)error;


@end

/**
 * 
 * @type api
 * @brief RTS房间
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCRTSRoom : NSObject
/**
 * 
 * @type callback
 */

@property(nonatomic, weak) id<ByteRTCRTSRoomDelegate> _Nullable delegateRts;

/**
 * 
 * @type api
 * @list 房间管理
 * @brief 退出并销毁调用 createRTSRoom:{@link #ByteRTCEngine#createRTSRoom:} 所创建的 RTS 房间实例。
 */

- (void)destroy;

/**
 * 
 * @type api
 * @list 房间管理
 * @brief 通过设置 ByteRTCRTSRoomDelegate{@link #ByteRTCRTSRoomDelegate}代理，可以监听此 `ByteRTCRTSRoom` 对象对应的回调事件。
 * @param roomDelegate 参见 ByteRTCRTSRoomDelegate{@link #ByteRTCRTSRoomDelegate}。
 * @return  <br>
 *        + 0: 调用成功。
 *        + < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 */

- (int)setRTCRoomDelegateRts:(id<ByteRTCRTSRoomDelegate> _Nullable)roomDelegate;


/**
 * 
 * @type api
 * @brief 离开房间。 <br>
 *        用户调用此方法离开房间，结束实时消息通信，释放所有通信相关的资源。
 * @return 方法调用结果。 <br>
 *        + 0: 方法调用成功 <br>
 *        + < 0: 方法调用失败 <br>
 * @note <br>
 *       + 可见的用户离开房间后，房间内其他用户会收到 rtsRoom:onUserLeave:reason:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onUserLeave:reason:} 回调通知；  <br>
 *       + 此方法是异步操作，调用返回时并没有真正退出房间。真正退出房间后，本地会收到 rtsRoom:OnLeaveRoom:{@link #ByteRTCRTSRoomDelegate#rtsRoom:OnLeaveRoom:} 回调通知。  <br>
 *       + 如果调用此方法后立即销毁引擎，SDK 将无法触发 rtsRoom:OnLeaveRoom:{@link #ByteRTCRTSRoomDelegate#rtsRoom:OnLeaveRoom:} 回调。
 * @list 房间管理
 */
 
- (int)leaveRoom;

#pragma mark - Message Methods

 /**
  * 
  * @type api
  * @brief 给房间内的所有其他用户发送文本消息。
  * @param message  <br>
  *        发送的文本消息内容。  <br>
  *        消息不超过 64KB。
  * @return 这次发送消息的编号，从 1 开始递增。
  * @note  <br>
  *      + 在发送房间内文本消息前，必须先调用 joinRTSRoom:userInfo:{@link #ByteRTCRTSRoom#joinRTSRoom:userInfo:} 加入房间。  <br>
  *      + 调用该函数后会收到一次 rtsRoom:onRoomMessageSendResult:error:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomMessageSendResult:error:} 回调，通知消息发送方发送成功或失败；  <br>
  *      + 若文本消息发送成功，则房间内远端用户会收到 rtsRoom:onRoomMessageReceived:message:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomMessageReceived:message:} 回调。
  * @list 消息
  */
 
- (int64_t)sendRoomMessage:(NSString * _Nonnull)message;
 /**
  * 
  * @type api
  * @brief 给房间内的所有其他用户发送二进制消息。
  * @param message  <br>
  *        用户发送的二进制广播消息  <br>
  *        消息不超过 64KB。
  * @return 这次发送消息的编号，从 1 开始递增。
  * @note  <br>
  *      + 在房间内广播二进制消息前，必须先调用 joinRTSRoom:userInfo:{@link #ByteRTCRTSRoom#joinRTSRoom:userInfo:} 加入房间。  <br>
  *      + 调用该函数后会收到一次 rtsRoom:onRoomMessageSendResult:error:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomMessageSendResult:error:} 回调，通知消息发送方发送成功或失败；  <br>
  *      + 若二进制消息发送成功，则房间内所有用户会收到 rtsRoom:onRoomBinaryMessageReceived:message:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomBinaryMessageReceived:message:} 回调。
  * @list 消息
  */
 
- (int64_t)sendRoomBinaryMessage:(NSData *  _Nonnull)message;

/**
 * 
 * @type api
 * @brief 更新 Token。 <br>
 *        收到 onTokenWillExpire:{@link #ByteRTCRoomDelegate#onTokenWillExpire:}，onPublishPrivilegeTokenWillExpire:{@link #ByteRTCRoomDelegate#onPublishPrivilegeTokenWillExpire:}，或 onSubscribePrivilegeTokenWillExpire:{@link #ByteRTCRoomDelegate#onSubscribePrivilegeTokenWillExpire:} 时，你必须重新获取 Token，并调用此方法更新 Token，以保证通话的正常进行。
 * @param token 重新获取的有效 Token。 <br>
 *        如果 Token 无效，你会收到 rtsRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomStateChanged:withUid:state:extraInfo:}，错误码是 `-1010`。
 * @return
 *        - 0：成功；
 *        - !0：失败。
 * @note 请勿同时调用 updateToken:{@link #ByteRTCRTSRoom#updateToken:} 和 joinRTSRoom:userInfo:{@link #ByteRTCRTSRoom#joinRTSRoom:userInfo:} 重新加入房间。
 * @list 房间管理
 */

- (int)updateToken:(NSString *_Nullable)token;

 /**
  * 
  * @type api
  * @brief 给房间内指定的用户发送点对点文本消息（P2P）。
  * @param userId <br>
  *        消息接收用户的 ID
  * @param message <br>
  *        发送的文本消息内容。 <br>
  *        消息不超过 64 KB。
  * @param config <br>
  *        消息发送的可靠/有序类型，参看 ByteRTCMessageConfig{@link #ByteRTCMessageConfig}
  * @return 这次发送消息的编号，从 1 开始递增。
  * @note
  *      - 在发送房间内文本消息前，必须先调用 joinRTSRoom:userInfo:{@link #ByteRTCRTSRoom#joinRTSRoom:userInfo:} 加入房间。
  *      - 调用该函数后会收到一次 rtsRoom:onUserMessageSendResult:error:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onUserMessageSendResult:error:} 回调，通知消息发送方发送成功或失败。
  *      - 若文本消息发送成功，则 uid 所指定的用户会收到 rtsRoom:onUserMessageReceived:message:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onUserMessageReceived:message:} 回调。
  * @list 消息
  */
 
- (int64_t)sendUserMessage:(NSString *_Nonnull)userId message:(NSString *_Nonnull)message config:(ByteRTCMessageConfig)config;
 /**
  * 
  * @type api
  * @brief 给房间内指定的用户发送点对点二进制消息（P2P）。
  * @param uid <br>
  *        消息接收用户的 ID
  * @param message <br>
  *        发送的二进制消息内容 <br>
  *        消息不超过 64KB。
  * @param config <br>
  *        消息发送的可靠/有序类型，参看 ByteRTCMessageConfig{@link #ByteRTCMessageConfig}。
  * @return 这次发送消息的编号，从 1 开始递增。
  * @note
  *      - 在发送房间内二进制消息前，必须先调用 joinRTSRoom:userInfo:{@link #ByteRTCRTSRoom#joinRTSRoom:userInfo:} 加入房间。
  *      - 调用该函数后会收到一次 rtsRoom:onUserMessageSendResult:error:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onUserMessageSendResult:error:} 回调，通知消息发送方发送成功或失败；
  *      - 若二进制消息发送成功，则 uid 所指定的用户会收到 rtsRoom:onUserBinaryMessageReceived:message:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onUserBinaryMessageReceived:message:} 回调。
  * @list 消息
  */
 
- (int64_t)sendUserBinaryMessage:(NSString * _Nonnull)uid message:(NSData * _Nonnull)message config:(ByteRTCMessageConfig)config;
/**
 * 
 * @type api
 * @brief 加入 RTS 房间。 <br>
 *        调用 createRTSRoom:{@link #ByteRTCEngine#createRTSRoom:} 创建房间后，调用此方法加入房间，同房间内其他用户进行音视频通话。
 * @param token 动态密钥，用于对进房用户进行鉴权验证。 <br>
 *        进入房间需要携带 Token。测试时可使用控制台生成临时 Token，正式上线需要使用密钥 SDK 在你的服务端生成并下发 Token。Token 有效期及生成方式参看[使用 Token 完成鉴权](70121)。 <br>
 *        使用不同 AppID 的 App 是不能互通的。 <br>
 *        请务必保证生成 Token 使用的 AppID 和创建引擎时使用的 AppID 相同，否则会导致加入房间失败。
 * @param userInfo 用户信息。参看 ByteRTCUserInfo{@link #ByteRTCUserInfo}。
 * @return 方法调用结果。 <br>
 *        -  0: 成功。触发以下回调：
 *          - 本端收到房间状态通知 rtsRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调。
 *        - -1: 参数无效
 *        - -2: 已经在房间内。接口调用成功后，只要收到返回值为 0 ，且未调用 leaveRoom{@link #ByteRTCRoom#leaveRoom} 成功，则再次调用进房接口时，无论填写的房间 ID 和用户 ID 是否重复，均触发此返回值。
 *        调用失败时，具体失败原因会通过 rtsRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调告知。
 * @note
 *        - 同一个 AppID 的同一个房间内，每个用户的用户 ID 必须是唯一的。如果两个用户的用户 ID 相同，则后加入房间的用户会将先加入房间的用户踢出房间，并且先加入房间的用户会收到 rtsRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调通知，错误类型为重复登录 ByteRTCErrorCodeDuplicateLogin。
 *        - 本地用户调用此方法加入房间成功后，会收到 rtsRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomStateChanged:withUid:state:extraInfo:} 回调通知。若本地用户同时为可见用户，加入房间时远端用户会收到 rtsRoom:onUserJoined:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onUserJoined:} 回调通知。
 *        - 用户加入房间成功后，在本地网络状况不佳的情况下，SDK 可能会与服务器失去连接，并触发 rtcEngine:onConnectionStateChanged:{@link #ByteRTCEngineDelegate#rtcEngine:onConnectionStateChanged:} 回调。此时 SDK 会自动重试，直到成功重连。重连成功后，本地会收到 rtsRoom:onRoomStateChanged:withUid:state:extraInfo:{@link #ByteRTCRTSRoomDelegate#rtsRoom:onRoomStateChanged:withUid:state:extraInfo:}。
 * @list 房间管理
 */

- (int)joinRTSRoom:(NSString *_Nullable)token userInfo:(ByteRTCUserInfo *_Nonnull)userInfo;

@end
