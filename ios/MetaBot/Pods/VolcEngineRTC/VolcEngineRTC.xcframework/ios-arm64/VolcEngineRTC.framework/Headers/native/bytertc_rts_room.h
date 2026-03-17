/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room Interface
 */

#pragma once

#include "rtc/bytertc_rts_defines.h"
#include "rtc/bytertc_media_defines.h"
#include "bytertc_rts_room_event_handler.h"

namespace bytertc {

/**
 * 
 * @type api
 * @brief RTS 房间接口
 */

class IRTSRoom {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IRTSRoom() {
    }

    /**
     * 
     * @type api
     * @list 房间管理
     * @brief 退出并销毁调用 createRTSRoom{@link #IRTCEngine#createRTSRoom} 所创建的 RTS 房间实例。
     */
    
    virtual void destroy() = 0;

    /**
     * 
     * @type api
     * @list 房间管理
     * @brief 离开 RTC 或 RTS 房间。  <br>
     *        用户调用此方法离开房间，结束通话过程，释放所有通话相关的资源。  <br>
     * @return  <br>
     *        + 0: 调用成功。
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note  <br>
     *       + 此方法是异步操作，调用返回时并没有真正退出房间。真正退出房间后，本地会收到 onLeaveRoom{@link #IRTSRoomEventHandler#onLeaveRoom} 回调通知。  <br>
     *       + 可见的用户离开房间后，房间内其他用户会收到 onUserLeave{@link #IRTSRoomEventHandler#onUserLeave} 回调通知。  <br>
     *       + 如果调用此方法后立即销毁引擎，SDK 将无法触发 onLeaveRoom{@link #IRTSRoomEventHandler#onLeaveRoom} 回调。  <br>
     *       + 调用 joinRoom{@link #IRTCRoom#joinRoom} 或 joinRTSRoom{@link #IRTSRoom#joinRTSRoom} 方法加入房间后，必须调用此方法离开房间，否则无法进入下一个房间。无论当前是否在房间内，都可以调用此方法。重复调用此方法没有负面影响。  <br>
     */
    
    virtual int leaveRoom() = 0;

    /**
     * 
     * @type api
     * @list 房间管理
     * @brief 设置 IRTSRoom{@link #IRTSRoom} 对象的事件句柄。
     *        通过设置事件句柄可以监听此 IRTSRoom{@link #IRTSRoom} 对象对应的房间的回调事件。
     * @param room_event_handler 回调处理器，详见 IRTSRoomEventHandler{@link #IRTSRoomEventHandler}
     * @return  <br>
     *        + 0: 调用成功。
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    
    virtual int setRTSRoomEventHandler(IRTSRoomEventHandler* room_event_handler) = 0;

    /**
     * 
     * @type api
     * @list 消息
     * @brief 给房间内的所有其他用户发送广播消息。
     * @param message  <br>
     *        用户发送的广播消息  <br>
     *        消息不超过 64KB。
     * @return 这次发送消息的编号，从 1 开始递增。
     * @note  <br>
     *       + 在发送房间内二进制消息前，必须先调用 joinRoom{@link #IRTCRoom#joinRoom} 加入房间。  <br>
     *       + 调用该函数后，会收到一次 onRoomMessageSendResult{@link #IRTSRoomEventHandler#onRoomMessageSendResult} 回调。  <br>
     *       + 同一房间内的其他用户会收到 onRoomMessageReceived{@link #IRTSRoomEventHandler#onRoomMessageReceived} 回调。
     */
    
    virtual int64_t sendRoomMessage(const char* message) = 0;
    /**
     * 
     * @type api
     * @list 消息
     * @brief 给房间内的所有其他用户发送广播消息。
     * @param size  <br>
     *        发送的二进制消息长度
     * @param message  <br>
     *        用户发送的二进制广播消息  <br>
     *        消息不超过 64KB。
     * @return 这次发送消息的编号，从 1 开始递增。
     * @note  <br>
     *       + 在发送房间内二进制消息前，必须先调用 joinRTSRoom{@link #IRTSRoom#joinRTSRoom} 加入房间。  <br>
     *       + 调用该函数后，会收到一次 onRoomMessageSendResult{@link #IRTSRoomEventHandler#onRoomMessageSendResult} 回调。  <br>
     *       + 同一房间内的其他用户会收到 onRoomBinaryMessageReceived{@link #IRTSRoomEventHandler#onRoomBinaryMessageReceived} 回调。
     */
    
    virtual int64_t sendRoomBinaryMessage(int size, const uint8_t* message) = 0;
    /**
     * 
     * @type api
     * @list 房间管理
     * @brief 创建/加入 RTS 房间。  <br>
     *        同一房间内的用户间可以相互通话。  <br>
     *        进房后重复调用无效，用户必须调用 `leaveRoom` 退出当前房间后，才能加入下一个房间。
     * @param token 用户登录必须携带的 Token，用于鉴权验证。 <br>
     *              测试时可使用[控制台](https:
     *              正式上线需要需要使用密钥 SDK 在你的服务端生成并下发 Token，roomId 置为空字符串（即 roomId:""），Token 有效期及生成方式参看[使用 Token 完成鉴权](https:
     * @param user_info 用户信息，参看 UserInfo{@link #UserInfo}。
     * @return 
     *        +  0: 成功  <br>
     *        + -1：token / user_info 包含了无效的参数。  <br>
     *        + -2：已经在房间内。
     * @note  <br>
     *       + 同一个 App ID 的同一个房间内，每个用户的用户 ID 必须是唯一的。如果两个用户的用户 ID 相同，则后进房的用户会将先进房的用户踢出房间，并且先进房的用户会收到 OnError{@link #IRTCEngineEventHandler#OnError} 回调通知，错误类型详见 ErrorCode{@link #ErrorCode} 中的 kErrorCodeDuplicateLogin。  <br>
     *       + 本地用户调用此方法加入房间成功后，会收到回调通知。  <br>
     *       + 用户加入房间成功后，在本地网络状况不佳的情况下，SDK 可能会与服务器失去连接，此时 SDK 将会自动重连。重连成功后，本地会收到  回调通知。如果加入房间的用户可见，远端用户会收到 onUserJoined{@link #IRTSRoomEventHandler#onUserJoined}。
     */
    
    virtual int joinRTSRoom(const char* token, const UserInfo& user_info) = 0;
    /**
     * 
     * @type api
     * @brief 更新 Token。 <br>
     *        收到 onTokenWillExpire{@link #IRTCRoomEventHandler#onTokenWillExpire}，onPublishPrivilegeTokenWillExpire{@link #IRTCRoomEventHandler#onPublishPrivilegeTokenWillExpire}， 或 onSubscribePrivilegeTokenWillExpire{@link #IRTCRoomEventHandler#onSubscribePrivilegeTokenWillExpire} 时，你必须重新获取 Token，并调用此方法更新 Token，以保证通话的正常进行。
     * @param token 重新获取的有效 Token。 <br>
     *        如果 Token 无效，你会收到 onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged}，错误码是 `-1010`。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *      - 3.50（不含）以前的版本中，Token 中的发布和订阅权限为保留参数，无实际控制权限；3.50 及以后版本开放 Token 发布订阅控制权限，如需通过 Token 控制连麦权限，请联系技术支持团队开通白名单后支持。
     *      - 请勿同时调用 updateToken{@link #IRTSRoom#updateToken} 和 joinRoom{@link #IRTCRoom#joinRoom} 方法更新 Token。若因 Token 过期或无效导致加入房间失败或已被移出房间，你应该在获取新的有效 Token 后调用 joinRoom{@link #IRTCRoom#joinRoom} 重新加入房间。
     * @list 房间管理
     */
    
    virtual int updateToken(const char* token) = 0;
    /**
     * 
     * @type api
     * @brief 给房间内指定的用户发送点对点文本消息（P2P）。
     * @param uid 消息接收用户的 ID
     * @param message 发送的文本消息内容。消息不超过 64 KB。
     * @param config 消息类型，参看 MessageConfig{@link #MessageConfig}。
     * @return 这次发送消息的编号，从 1 开始递增。
     * @note
     *       - 在发送房间内文本消息前，必须先调用 joinRoom{@link #IRTCRoom#joinRoom} 或 joinRTSRoom{@link #IRTSRoom#joinRTSRoom} 加入房间。
     *       - 调用该函数后会收到一次 onUserMessageSendResult{@link #IRTSRoomEventHandler#onUserMessageSendResult} 回调，通知消息发送方发送成功或失败；
     *       - 若文本消息发送成功，则 uid 所指定的用户会收到 onUserMessageReceived{@link #IRTSRoomEventHandler#onUserMessageReceived} 回调。
     * @list 消息
     */
    
    virtual int64_t sendUserMessage(const char* uid, const char* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;
    /**
     * 
     * @type api
     * @brief 给房间内指定的用户发送点对点二进制消息（P2P）。
     * @param uid 消息接收用户的 ID
     * @param length 二进制字符串的长度。
     * @param message 二进制消息的内容。消息不超过 64KB。
     * @param config 消息类型，参看 MessageConfig{@link #MessageConfig}。
     * @return 这次发送消息的编号，从 1 开始递增。
     * @note
     *       - 在发送房间内二进制消息前，必须先调用 joinRoom{@link #IRTCRoom#joinRoom} 或 joinRTSRoom{@link #IRTSRoom#joinRTSRoom} 加入房间。
     *       - 调用该函数后会收到一次 onUserMessageSendResult{@link #IRTSRoomEventHandler#onUserMessageSendResult} 回调，通知消息发送方发送成功或失败；
     *       - 若二进制消息发送成功，则 uid 所指定的用户会收到 onUserBinaryMessageReceived{@link #IRTSRoomEventHandler#onUserBinaryMessageReceived} 回调。
     * @list 消息
     */
    
    virtual int64_t sendUserBinaryMessage(const char* uid, int length, const uint8_t* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;
};

}  
