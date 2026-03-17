/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room Event Handler Interface
*/

#pragma once

namespace byterts {
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
     * @region 房间管理
     * @brief 离开房间回调。  <br>
     *        用户调用 leaveRoom{@link #IRTSRoom#leaveRoom} 方法后，SDK 会停止所有的发布订阅流，并在释放所有与通话相关的音频资源后，通过此回调通知用户离开房间成功。  <br>
     * @note  <br>
     *       + 用户调用 leaveRoom{@link #IRTSRoom#leaveRoom} 方法离开房间后，如果立即调用 destroy{@link #IRTSRoom#destroy} 销毁房间实例方法销毁 RTS 引擎，则将无法收到此回调事件。  <br>
     *       + 离开房间后，如果 App 需要使用系统音频设备，则建议收到此回调后再初始化音频设备，否则可能由于 SDK 占用音频设备而导致初始化失败。  <br>
     */
    
    virtual void onLeaveRoom() {
    }

    /**
     * 
     * @deprecated since 3.41 and will be deleted in 3.51, user  instead.
     * @type callback
     * @region 房间管理
     * @brief SDK 发生警告回调。  <br>
     *   SDK内部遇到可恢复错误时，在尝试自动恢复的同时，会通过此回调通知用户。此回调事件仅用作通知。  <br>
     * @param warn 警告码，参看 WarningCode{@link #WarningCode}。  <br>
     */
    
    virtual void onRoomWarning(int warn) {
        (void)warn;
    }

    /**
     * 
     * @deprecated since 3.41 and will be deleted in 3.51.
     * @type callback
     * @region 房间管理
     * @brief SDK 发生错误回调。  <br>
     *        SDK 内部遇到不可恢复错误时，会通过此回调通知 App，需要 App 根据错误码进行操作或提示用户。  <br>
     * @param err 错误码，参看 ErrorCode{@link #ErrorCode}。  <br>
     */
    
    virtual void onRoomError(int err) {
        (void)err;
    }

    /**
     * 
     * @type callback
     * @region 房间管理
     * @brief 远端用户加入房间的回调。
     * @param uid 加入房间的用户 ID  <br>
     * @param elapsed 此参数无意义
     */
    
    virtual void onUserJoined(const char* uid, int elapsed) {
        (void)uid;
        (void)elapsed;
    }

    /**
     * 
     * @type callback
     * @brief 远端用户离开房间时，本地用户会收到此事件
     * @param uid 离开房间的远端用户 ID。
     * @param reason 用户离开房间的原因。参看 UserOfflineReason{@link #UserOfflineReason}。
     */
    
    virtual void onUserLeave(const char* uid, bytertc::UserOfflineReason reason) {
        (void)uid;
        (void)reason;
    }

    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 接收到房间内广播消息的回调。<br>
     * 当房间内的其他用户调用 sendRoomMessage{@link #IRTSRoom#sendRoomMessage} 时，你会收到此回调。
     * @param uid 发送该消息的用户的 ID。
     * @param message 收到的消息内容。
     */
    
    virtual void onRoomMessageReceived(const char* uid, const char* message) {
        (void)uid;
        (void)message;
    }

    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 接收到房间内二进制广播消息的回调。<br>
     * 同一房间内其他用户调用 sendRoomBinaryMessage{@link #IRTSRoom#sendRoomBinaryMessage} 发送二进制广播消息时会收到该回调。
     * @param uid 发送该消息的用户的 ID。
     * @param size 收到的二进制消息长度。
     * @param message 收到的二进制消息内容。
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
     * @param msgid 本条消息的 ID。
     * @param error 消息发送结果。详见 RoomMessageSendResult{@link #RoomMessageSendResult}。
     */
    
    virtual void onRoomMessageSendResult(int64_t msgid, int error) {
        (void)msgid;
        (void)error;
    }
};

}  

