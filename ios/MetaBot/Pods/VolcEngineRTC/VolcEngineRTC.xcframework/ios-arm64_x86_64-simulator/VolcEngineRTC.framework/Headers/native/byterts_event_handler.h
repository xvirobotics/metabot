/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Event Handler Lite
*/

#pragma once

#include "rtc/bytertc_rts_defines.h"

namespace byterts {
/**
 * 
 * @type callback
 * @brief 实时信令事件回调接口
 */

class IRTSEventHandler {
public:

    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~IRTSEventHandler() {
    }

    /**
     * 
     * @type callback
     * @region 警告码
     * @brief 当内部发生警告事件时触发该回调
     * @param warn
     *        警告标识码，详见:WarningCode{@link #WarningCode}
     */
    
    virtual void onWarning(int warn) {
        (void)warn;
    }

    /**
     * 
     * @type callback
     * @region 错误码
     * @brief 当 SDK 内部发生不可逆转错误时触发该回调。
     * @param err 错误标识码，参看 ErrorCode{@link #ErrorCode}
     */
    
    virtual void onError(int err) {
        (void)err;
    }

   /**
    * 
    * @valid since 3.52
    * @type callback
    * @brief 当访问插件失败时，收到此回调。
    *        RTC SDK 将一些功能封装成插件。当使用这些功能时，如果插件不存在，功能将无法使用。
    * @param extension_name 插件名字
    * @param msg 失败说明
    */
   
     void onExtensionAccessError(const char* extension_name, const char* msg) {
        (void)extension_name;
        (void)msg;
     }

    /**
     * 
     * @type callback
     * @region 引擎管理
     * @brief 端监控日志回调。当产生一个端监控事件时触发该回调。
     * @param log_type  <br>
     *        事件类型。目前类型固定为 "live_webrtc_monitor_log"。
     * @param log_content  <br>
     *        端监控日志内容。
     */
    
    virtual void onLogReport(const char* log_type, const char* log_content) {
        (void)log_type;
        (void)log_content;
    }

    /**
     * 
     * @type callback
     * @region 引擎管理
     * @brief SDK 与信令服务器连接状态改变回调。连接状态改变时触发。
     * @param state 当前 SDK 与信令服务器的连接状态，详见 ConnectionState{@link #ConnectionState}。
     * @note 更多信息参见 [连接状态提示](https:
     */
    
    virtual void onConnectionStateChanged(bytertc::ConnectionState state) {
    }

    /**
     * 
     * @type callback
     * @region 引擎管理
     * @brief SDK 当前网络连接类型改变回调。当 SDK 的当前网络连接类型发生改变时回调该事件。
     * @param type  <br>
     *        SDK 当前的网络连接类型，详见：NetworkType{@link #NetworkType} 。
     */
    
    virtual void onNetworkTypeChanged(bytertc::NetworkType type) {
    }

    /**
     * 
     * @type callback
     * @region 引擎管理
     * @brief 周期性（2s）地发出回调，报告当前cpu与memory使用率
     * @param stats 返回包含当前系统状态信息的结构体，详见 SysStats{@link #SysStats}
     */
    
    virtual void onSysStats(const bytertc::SysStats& stats) {
    }

    /**
     * 
     * @type callback
     * @region 引擎管理
     * @brief 创建房间失败回调。
     * @param room_id 房间 ID。
     * @param error_code 创建房间错误码，具体原因参看 ErrorCode{@link #ErrorCode}。
     */
    
    virtual void onCreateRoomStateChanged(const char* room_id, int error_code) {
        (void)room_id;
        (void)error_code;
    }

    /**
     * 
     * @type callback
     * @deprecated 在3.52及之后废弃，将在3.57删除，使用 onLocalProxyStateChanged{@link #IRTSEventHandler#onLocalProxyStateChanged} 替换
     * @region 代理回调
     * @brief HTTP 代理连接状态改变时，收到该回调。
     * @param state 当前 HTTP 代理连接状态，详见 HttpProxyState{@link #HttpProxyState}
     */
    
        virtual void onHttpProxyState(int state) {
    }

    /**
     * 
     * @type callback
     * @deprecated 在3.52及之后废弃，将在3.57删除，使用 onLocalProxyStateChanged{@link #IRTSEventHandler#onLocalProxyStateChanged} 替换
     * @region 代理回调
     * @brief HTTPS 代理连接状态改变时，收到该回调。
     * @param state 当前 HTTPS 代理连接状态，详见 HttpProxyState{@link #HttpProxyState}
     */
    
    virtual void onHttpsProxyState(int state) {
    }

    /**
     * 
     * @type callback
     * @deprecated 在3.52及之后废弃，将在3.57删除，使用 onLocalProxyStateChanged{@link #IRTSEventHandler#onLocalProxyStateChanged} 替换
     * @region 代理回调
     * @brief Socks5 代理状态改变时，收到该回调。
     * @param state SOCKS5 代理连接状态，详见 Socks5ProxyState{@link #Socks5ProxyState}
     * @param cmd 代理连接的每一步操作命令
     * @param proxy_address 代理地址信息
     * @param local_address 当前连接使用的本地地址
     * @param remote_address 远端的连接地址
     */
    
    virtual void onSocks5ProxyState(int state,
        const char* cmd,
        const char* proxy_address,
        const char* local_address,
        const char* remote_address) {
    }

    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 登录结果回调
     * @param uid  <br>
     *        登录用户 ID
     * @param error_code  <br>
     *        登录结果  <br>
     *        详见 LoginErrorCode{@link #LoginErrorCode}。
     * @param elapsed  <br>
     *        从调用 login{@link #IRTS#login} 接口开始到返回结果所用时长  <br>
     *        单位为 ms。
     * @note 调用 login{@link #IRTS#login} 后，会收到此回调。
     */
    
    virtual void onLoginResult(const char* uid, int error_code, int elapsed) {
        (void)uid;
        (void)error_code;
        (void)elapsed;
    }
    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 登出结果回调
     * @param reason 用户登出的原因，参看 LogoutReason{@link #LogoutReason}
     * @note 在以下两种情况下会收到此回调：调用 logout{@link #IRTS#logout} 接口主动退出；或其他用户以相同 UserId 进行 `login` 导致本地用户被动登出。
     */
    
    virtual void onLogout(bytertc::LogoutReason reason) {
    }
    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 设置应用服务器参数的返回结果
     * @param error <br>
     *        设置结果  <br>
     *        + 返回 200，设置成功  <br>
     *        + 返回其他，设置失败
     * @note 调用 setServerParams{@link #IRTS#setServerParams} 后，会收到此回调。
     */
    
    virtual void onServerParamsSetResult(int error) {
        (void)error;
    }
    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 查询对端或本端用户登录状态的返回结果
     * @param peer_user_id 需要查询的用户 ID。
     * @param status 查询的用户登录状态。详见 UserOnlineStatus{@link #UserOnlineStatus}。
     * @note 必须先调用 getPeerOnlineStatus{@link #IRTS#getPeerOnlineStatus}，才能收到此回调。
     */
    
    virtual void onGetPeerOnlineStatus(const char* peer_user_id, int status) {
        (void)peer_user_id;
        (void)status;
    }
    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 收到房间外用户调用 sendMessage{@link #IRTS#sendMessage} 发来的文本消息时，会收到此回调
     * @param uid 消息发送者 ID。
     * @param message  <br>
     *        收到的文本消息内容
     */
    
    virtual void onMessageReceived(const char* uid, const char* message) {
        (void)uid;
        (void)message;
    }
    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 收到房间外用户调用 sendBinaryMessage{@link #IRTS#sendBinaryMessage} 发来的二进制消息时，会收到此回调。
     * @param uid 消息发送者 ID。
     * @param size 二进制消息长度。
     * @param message 收到的二进制消息内容。
     */
    
    virtual void onBinaryMessageReceived(const char* uid, int size, const uint8_t* message) {
        (void)uid;
        (void)size;
        (void)message;
    }
    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 给房间外指定的用户发送消息的回调。
     * @param msgid 消息的 ID。  <br>
     *        所有的 P2P 和 P2Server 消息共用一个 ID 序列。
     * @param error 消息发送结果。详见 UserMessageSendResult{@link #UserMessageSendResult}。
     * @note 当调用 sendMessage{@link #IRTS#sendMessage} 或 sendBinaryMessage{@link #IRTS#sendBinaryMessage} 发送消息后，会收到此回调。
     */
    
    virtual void onMessageSendResult(int64_t msgid, int error) {
        (void)msgid;
        (void)error;
    }

    /**
     * 
     * @type callback
     * @region 实时消息通信
     * @brief 给应用服务器发送消息的回调
     * @param msgid 本条消息的 ID  <br>
     *        所有的 P2P 和 P2Server 消息共用一个 ID 序列。
     * @param error 消息发送结果，详见 UserMessageSendResult{@link #UserMessageSendResult}。
     * @param message 应用服务器收到 HTTP 请求后，在 ACK 中返回的信息。
     * @note 当调用 sendServerMessage{@link #IRTS#sendServerMessage} 或 sendServerBinaryMessage{@link #IRTS#sendServerBinaryMessage} 发送消息后，会收到此回调。
     */
    
    virtual void onServerMessageSendResult(int64_t msgid, int error, const bytertc::ServerACKMsg& msg) {
        (void)msgid;
        (void)error;
        (void)msg;
    }

    /**
     * 
     * @type callback
     * @brief 调用 StartCloudProxy{@link #IRtcEngineLite#StartCloudProxy} 开启云代理，SDK 首次成功连接云代理服务器时，回调此事件。
     * @param interval 从开启云代理到连接成功经过的时间，单位为 ms
     */
    
    virtual void onCloudProxyConnected(int interval) {
        (void)interval;
    }

    /**
     * 
     * @type callback
     * @brief 调用 setLocalProxy{@link #IRTS#setLocalProxy} 设置本地代理，SDK对代理连接的状态
     * 回调此接口。
     * @param local_proxy_type 本地代理类型：sock5 或 httptunnel
     * @param local_proxy_state 本地代理状态。参看 LocalProxyState{@link #LocalProxyState}。  <br>
     * @param local_proxy_error 本地代理错误。参看 LocalProxyError{@link #LocalProxyError}。  <br>
     */
    
    virtual void onLocalProxyStateChanged(bytertc::LocalProxyType local_proxy_type, bytertc::LocalProxyState local_proxy_state, bytertc::LocalProxyError local_proxy_error) {
        (void)local_proxy_type;
        (void)local_proxy_state;
        (void)local_proxy_error;
    }
};

} 

