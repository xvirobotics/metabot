/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room Event Handler Interface
*/

#pragma once

#include "rtc/bytertc_defines.h"

namespace bytertc {
/**
 * 
 * @type callback
 * @brief 实时信令事件回调接口
 */

class IRTSRoomEventHandler {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IRTSRoomEventHandler() {}

    /**
     * 
     * @type callback
     * @region 房间管理
     * @brief 房间状态改变回调，加入房间、异常退出房间、发生房间相关的警告或错误时会收到此回调。
     * @param room_id 房间 ID。
     * @param uid 用户 ID。
     * @param state 房间状态码。  <br>
     *              + 0: 加入房间成功。  <br>
     *              + !0: 加入房间失败、异常退房、发生房间相关的警告或错误。具体原因参看 ErrorCode{@link #ErrorCode} 及 WarningCode{@link #WarningCode}。
     * @param extra_info 额外信息，如 `{"elapsed":1187,"join_type":0}`。
     *                  `join_type`表示加入房间的类型，`0`为首次进房，`1`为重连进房。
     *                  `elapsed`表示加入房间耗时，即本地用户从调用 joinRoom{@link #IRTCRoom#joinRoom} 到加入房间成功所经历的时间间隔，单位为 ms。
     * @list 房间管理
     */
    
    virtual void onRoomStateChanged(const char* room_id, const char* uid,
            int state, const char* extra_info) {
        (void)room_id;
        (void)state;
        (void)extra_info;
    }

    /**
     * 
     * @type callback
     * @brief 离开房间回调。  <br>
     *        用户调用 leaveRoom{@link #IRTSRoom#leaveRoom} 方法后，SDK 会停止所有的发布订阅流，并在释放所有与通话相关的音频资源后，通过此回调通知用户离开房间成功。  <br>
     * @param stats 保留参数，目前为空。
     * @note  <br>
     *       + 用户调用 leaveRoom{@link #IRTSRoom#leaveRoom} 方法离开房间后，如果立即调用 destroy{@link #IRTSRoom#destroy} 销毁房间实例方法销毁 RTS 引擎，则将无法收到此回调事件。  <br>
     *       + 离开房间后，如果 App 需要使用系统音频设备，则建议收到此回调后再初始化音频设备，否则可能由于 SDK 占用音频设备而导致初始化失败。  <br>
     * @list 房间管理 
     */
    
    virtual void onLeaveRoom(const RtcRoomStats& stats) {
        (void)stats;
    }

    /**
     * 
     * @type callback
     * @region 房间管理
     * @brief 远端用户加入房间的回调。
     * @param user_info 用户信息，详见 UserInfo{@link #UserInfo}
     * @list 房间管理
     */
    
    virtual void onUserJoined(const UserInfo& user_info) {
        (void)user_info;
    }

    /**
     * 
     * @type callback
     * @brief 远端用户离开房间时，本地用户会收到此事件
     * @param uid 离开房间的远端用户 ID。  <br>
     * @param reason 用户离开房间的原因。参看 UserOfflineReason{@link #UserOfflineReason}。
     * @list 房间管理
     */
    
    virtual void onUserLeave(const char* uid, UserOfflineReason reason) {
        (void)uid;
        (void)reason;
    }

    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 接收到房间内广播消息的回调。
     * @param uid 消息发送者 ID。
     * @param message 收到的消息内容。
     * @note 同一房间内其他用户调用 sendRoomMessage{@link #IRTSRoom#sendRoomMessage} 发送广播消息时会收到该回调。
     * @list 消息
     */
    
    virtual void onRoomMessageReceived(const char* uid, const char* message) {
        (void)uid;
        (void)message;
    }

    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 接收到房间内二进制广播消息的回调。
     * @param uid 消息发送者 ID。
     * @param size <br>
     *        收到的二进制消息长度
     * @param message 收到的二进制消息内容。
     * @note 同一房间内其他用户调用 sendRoomBinaryMessage{@link #IRTSRoom#sendRoomBinaryMessage} 发送二进制广播消息时会收到该回调。
     * @list 消息
     */
    
    virtual void onRoomBinaryMessageReceived(const char* uid, int size, const uint8_t* message) {
        (void)uid;
        (void)size;
        (void)message;
    }

    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 当调用 sendRoomMessage{@link #IRTSRoom#sendRoomMessage} 函数发送消息后，回调此条消息的发送结果（反馈）。
     * @param msgid 消息的 ID。
     * @param error 消息发送结果。详见 RoomMessageSendResult{@link #RoomMessageSendResult}。
     * @list 消息
     */
    
    virtual void onRoomMessageSendResult(int64_t msgid, int error) {
        (void)msgid;
        (void)error;
    }

    /**
     * 
     * @type callback
     * @brief > 该接口将于 3.64 onUserMessageReceived{@link #IRTSRoomEventHandler#onUserMessageReceived-2} 代替。
     * @brief 收到来自房间中其他用户通过 sendUserMessage{@link #IRTSRoom#sendUserMessage} 发来的点对点文本消息时，会收到此回调。
     * @param uid 消息发送者 ID 。
     * @param message 收到的文本消息内容。
     * @note
     *       你必须先调用 sendUserMessage{@link #IRTSRoom#sendUserMessage} 方法，才能收到该回调。
     * @list 消息
     */
    
    virtual void onUserMessageReceived(const char* uid, const char* message) {
        (void)uid;
        (void)message;
    }

    /**
     * 
     * @type callback
     * @brief 收到来自房间中其他用户通过 sendUserBinaryMessage{@link #IRTSRoom#sendUserBinaryMessage} 发来的点对点二进制消息时，会收到此回调。<br>
     *        > 该接口将于 3.64 onUserBinaryMessageReceived{@link #IRTSRoomEventHandler#onUserBinaryMessageReceived-2} 代替。
     * @param uid 消息发送者 ID。
     * @param size 收到的消息长度。
     * @param message 收到的二进制消息内容。
     * @list 消息
     */
    
    virtual void onUserBinaryMessageReceived(const char* uid, int size, const uint8_t* message) {
        (void)uid;
        (void)size;
        (void)message;
    }

    /**
     * 
     * @type callback
     * @brief 向房间内单个用户发送文本或二进制消息后（P2P），消息发送方会收到该消息发送结果回调。
     * @param msgid 消息的 ID。
     * @param error 文本或二进制消息发送结果，详见 UserMessageSendResult{@link #UserMessageSendResult}
     * @note
     *       你必须先调用 sendUserMessage{@link #IRTSRoom#sendUserMessage} 或 sendUserBinaryMessage{@link #IRTSRoom#sendUserBinaryMessage} 接口，才能收到此回调。
     * @list 消息
     */
    
    virtual void onUserMessageSendResult(int64_t msgid, int error) {
        (void)msgid;
        (void)error;
    }

    /**
     * 
     * @type callback
     * @brief 接收到房间内广播消息的回调。<br>
     *        同一房间内其他用户调用 sendUserMessage{@link #IRTSRoom#sendUserMessage} 发送广播消息时会收到该回调。
     * @param msgid 消息编号。
     * @param uid 消息发送者的 ID。
     * @param message 收到的消息内容。
     * @list 消息
     */
    
    virtual void onRoomMessageReceived(int64_t msgid, const char* uid, const char* message) {
        (void)msgid;
        (void)uid;
        (void)message;
    }
    /**
     * 
     * @type callback
     * @brief 收到房间内广播二进制消息的回调。 <br>
     *        房间内其他用户调用 sendRoomBinaryMessage{@link #IRTSRoom#sendRoomBinaryMessage} 发送广播二进制消息时，收到此回调。
     * @param msgid 消息编号。
     * @param uid 消息发送者 ID。
     * @param message 收到的二进制消息内容。
     * @param size 收到的二进制消息长度。
     * @note 同一房间内其他用户调用 sendRoomBinaryMessage{@link #IRTSRoom#sendRoomBinaryMessage} 发送二进制广播消息时会收到该回调。
     * @list 消息
     */
    
    virtual void onRoomBinaryMessageReceived(int64_t msgid, const char* uid, const uint8_t* message, int size) {
        (void)msgid;
        (void)uid;
        (void)message;
        (void)size;
    }
    /**
     * 
     * @type callback
     * @brief 收到来自房间中其他用户通过 sendUserMessage{@link #IRTSRoom#sendUserMessage} 发来的点对点文本消息时，会收到此回调。
     * @param msgid 消息编号。
     * @param uid 消息发送者 ID。
     * @param message 收到的文本消息内容。
     * @note
     *        你必须先调用 sendUserMessage{@link #IRTSRoom#sendUserMessage} 方法，才能收到该回调。
     * @list 消息
     */
    
    virtual void onUserMessageReceived(int64_t msgid, const char* uid, const char* message) {
        (void)msgid;
        (void)uid;
        (void)message;
    }
    /**
     * 
     * @type callback
     * @brief 收到来自房间中其他用户通过 sendUserBinaryMessage{@link #IRTSRoom#sendUserBinaryMessage} 发来的点对点二进制消息时，会收到此回调。
     * @param msgid 消息编号。
     * @param uid 消息发送者 ID 。
     * @param message 收到的二进制消息内容。
     * @param size 消息长度。
     * @list 消息
     */
    
    virtual void onUserBinaryMessageReceived(int64_t msgid, const char* uid, const uint8_t* message, int size) {
        (void)msgid;
        (void)uid;
        (void)message;
        (void)size;
    }
};

}  

