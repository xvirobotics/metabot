/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room Interface
 */

#pragma once

#include "rtc/bytertc_rts_defines.h"
#include "byterts_room_event_handler.h"

namespace byterts {

/**
 * 
 * @type api
 * @brief 房间接口
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
     * @brief 退出并销毁调用 createRoom{@link #IRTS#createRoom} 所创建的房间实例。
     */
    
    virtual void destroy() = 0;

    /**
     * 
     * @type api
     * @list 房间管理
     * @brief 创建/加入 RTS 房间：房间不存在时即创建房间；房间存在时，未加入房间的用户可加入这个房间。  <br>
     *        同一房间内的用户间可以相互通话。  <br>
     *        进房后重复调用无效，用户必须调用 `leaveRoom` 退出当前房间后，才能加入下一个房间。
     * @return  <br>
     *        +  0: 成功  <br>
     *        + -1：room_id / user_info.uid 包含了无效的参数。  <br>
     *        + -2：已经在房间内。
     * @note  <br>
     *       + 同一个 App ID 的同一个房间内，每个用户的用户 ID 必须是唯一的。如果两个用户的用户 ID 相同，则后进房的用户会将先进房的用户踢出房间，并且先进房的用户会收到 OnError{@link #IRTCAudioEventHandler#OnError} 回调通知，错误类型详见 ErrorCode{@link #ErrorCode} 中的 kErrorCodeDuplicateLogin。  <br>
     *       + 本地用户调用此方法加入房间成功后，会收到  回调通知。  <br>
     *       + 用户加入房间成功后，在本地网络状况不佳的情况下，SDK 可能会与服务器失去连接，此时 SDK 将会自动重连。重连成功后，本地会收到  回调通知。如果加入房间的用户可见，远端用户会收到 onUserJoined{@link #IRTSRoomEventHandler#onUserJoined}。
     */
    
    virtual int joinRoom() = 0;

    /**
     * 
     * @type api
     * @list 房间管理
     * @brief 离开 RTS 房间。  <br>
     *        用户调用此方法离开房间，结束通话过程，释放所有通话相关的资源。  <br>
     * @return  <br>
     *        + 0: 调用成功。
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note  <br>
     *       + 此方法是异步操作，调用返回时并没有真正退出房间。真正退出房间后，本地会收到 onLeaveRoom{@link #IRTSRoomEventHandler#onLeaveRoom} 回调通知。  <br>
     *       + 可见的用户离开房间后，房间内其他用户会收到 onUserLeave{@link #IRTSRoomEventHandler#onUserLeave} 回调通知。  <br>
     *       + 如果调用此方法后立即销毁引擎，SDK 将无法触发 onLeaveRoom{@link #IRTSRoomEventHandler#onLeaveRoom} 回调。  <br>
     *       + 调用 joinRoom{@link #IRTCRoom#joinRoom} 方法加入房间后，必须调用此方法离开房间，否则无法进入下一个房间。无论当前是否在房间内，都可以调用此方法。重复调用此方法没有负面影响。  <br>
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
     * @list 房间管理
     * @brief 给房间内的所有其他用户发送广播消息。
     * @param message 用户发送的广播消息，消息不超过 62KB。
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
     * @param size 发送的二进制消息长度
     * @param message 用户发送的二进制广播消息  <br>
     *        消息不超过 64KB。
     * @return 这次发送消息的编号，从 1 开始递增。
     * @note  <br>
     *       + 在发送房间内二进制消息前，必须先调用 joinRoom{@link #IRTCRoom#joinRoom} 加入房间。  <br>
     *       + 调用该函数后，会收到一次 onRoomMessageSendResult{@link #IRTSRoomEventHandler#onRoomMessageSendResult} 回调。  <br>
     *       + 同一房间内的其他用户会收到 onRoomBinaryMessageReceived{@link #IRTSRoomEventHandler#onRoomBinaryMessageReceived} 回调。
     */
    
    virtual int64_t sendRoomBinaryMessage(int size, const uint8_t* message) = 0;
};

}  
