/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Common Defines
 */

#pragma once

#ifdef WIN32
#define BYTERTC_API extern "C" __declspec(dllexport)
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_MAC && !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#define BYTERTC_API __attribute__((__visibility__("default"))) extern "C"
#else
#define BYTERTC_API __attribute__((__visibility__("default")))
#endif
#else
#define BYTERTC_API __attribute__((__visibility__("default")))
#endif

#ifdef __GNUC__
#    define GCC_VERSION_AT_LEAST(x,y) (__GNUC__ > (x) || __GNUC__ == (x) && __GNUC_MINOR__ >= (y))
#else
#    define GCC_VERSION_AT_LEAST(x,y) 0
#endif

#if GCC_VERSION_AT_LEAST(3,1)
#    define BYTERTC_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#    define BYTERTC_DEPRECATED __declspec(deprecated)
#else
#    define BYTERTC_DEPRECATED
#endif

#ifdef WIN32
#define BYTERTC_STATIC_API __declspec(dllexport)
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_MAC && !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#define BYTERTC_STATIC_API __attribute__((__visibility__("default")))
#else
#define BYTERTC_STATIC_API __attribute__((__visibility__("default")))
#endif
#else
#define BYTERTC_STATIC_API __attribute__((__visibility__("default")))
#endif

#include <stdint.h>
#include <stddef.h>

namespace bytertc {


/**
 * 
 * @hidden currently not available
 */
 
enum RtsReturnStatus {

    kRtsReturnStatusSuccess = 0,

    kRtsReturnStatusFailure = -1,

    kRtsReturnStatusParameterErr = -2,

    kRtsReturnStatusWrongState = -3,

    kRtsReturnStatusHasInRoom = -4,

    kRtsReturnStatusHasInLogin = -5,

    kRtsReturnStatusRoomIdInUse = -8,

};

/**
 * 
 * @type keytype
 * @brief 用户离开房间的原因。
 */

enum UserOfflineReason {
    /**
     * 
     * @brief 远端用户调用 `leaveRoom` 方法退出房间。
     */
    
    kUserOfflineReasonQuit = 0,
    /**
     * 
     * @brief 远端用户因网络等原因掉线。
     */
    
    kUserOfflineReasonDropped = 1,
    /**
     * 
     * @brief 远端用户切换至隐身状态。
     */
    
    kUserOfflineReasonSwitchToInvisible = 2,
    /**
     * 
     * @brief 远端用户被踢出出房间。 <br>
     *        因调用踢出用户的 OpenAPI，远端用户被踢出房间。
     */
    
    kUserOfflineReasonKickedByAdmin = 3,
};

/**
 * 
 * @type keytype
 * @brief 用户加入房间的类型。
 */

enum JoinRoomType {
    /**
     * 
     * @brief 首次加入房间。用户手动调用 `joinRoom` 加入房间。
     */
    
    kJoinRoomTypeFirst = 0,
    /**
     * 
     * @brief 重新加入房间。用户网络较差，失去与服务器的连接，进行重连时收到加入成功。
     */
    
    kJoinRoomTypeReconnected = 1,
};

/**
 * 
 * @type keytype
 * @brief 用户登录的类型
 */

enum LoginType {
    /**
     * 
     * @brief 首次登录。用户手动调用 `login`，收到登录成功。
     */
    
    kLoginTypeFirst = 0,
    /**
     * 
     * @brief 重连。用户网络较差，失去与服务器的连接，SDK 自动重连登录成功。
     */
    
    kLoginTypeReconnected = 1,
};

/**
 * 
 * @type errorcode
 * @brief 登录结果 <br>
 *        调用 `login` 登录的结果，会通过 `onLoginResult` 回调通知用户。
 */

enum LoginErrorCode {
    /**
     * 
     * @brief 调用 `login` 方法登录成功
     */
    
    kLoginErrorCodeSuccess = 0,
    /**
     * 
     * @brief 调用 `login` 方法时使用的 Token 无效或过期失效，需要用户重新获取 Token。
     */
    
    kLoginErrorCodeInvalidToken = -1000,
    /**
     * 
     * @brief 登录错误 <br>
     *        调用 `login` 方法时发生未知错误导致登录失败，需要重新登录。
     */
    
    kLoginErrorCodeLoginFailed = -1001,
    /**
     * 
     * @brief 调用 `login` 方法时传入的用户 ID 有问题。
     */
    
    kLoginErrorCodeInvalidUserId = -1002,
    /**
     * 
     * @brief 调用 `login` 登录时服务端出错。
     */
    
    kLoginErrorCodeServerError = -1003,
    /**
     * 
     * @brief 当前为受限模式，禁止 `login` 登录。
     */
    
    kLoginErrorCodeLimitedMode = -1004,
};

/**
 * 
 * @type keytype
 * @brief 用户登出的原因
 */

enum LogoutReason {
    /**
     * 
     * @brief 用户主动退出 <br>
     *        用户调用 `logout` 接口登出，或者销毁引擎登出。
     */
    
    kLogoutReasonLogout = 0,
    /**
     * 
     * @brief 用户被动退出 <br>
     *        另一个用户以相同 UserId 进行了 `login`，导致本端用户被踢出。
     */
    
    kLogoutReasonDuplicateLogin = 1,
};

/**
 * 
 * @type keytype
 * @brief 用户在线状态。
 */

enum UserOnlineStatus {
    /**
     * 
     * @brief 对端用户离线 <br>
     *        对端用户已调用 `logout`，或者没有调用 `login` 进行登录。
     */
    
    kUserOnlineStatusOffline = 0,
    /**
     * 
     * @brief 对端用户在线 <br>
     *        对端用户调用 `login` 登录，并且连接状态正常。
     */
    
    kUserOnlineStatusOnline = 1,
    /**
     * 
     * @brief 无法获取对端用户在线状态 <br>
     *        发生级联错误、对端用户在线状态异常时返回。
     */
    
    kUserOnlineStatusUnreachable = 2,
};

/**
 * 
 * @type keytype
 * @brief 服务端消息发送结果回调内容
 */

struct ServerACKMsg {
    /**
     * 
     * @brief 回调消息长度
     */
    
    int length;
    /**
     * 
     * @brief 回调消息内容
     */
    
    char* ack_msg;
};

/**
 * 
 * @type keytype
 * @brief 消息类型
 */

enum MessageConfig {
    /**
     * 
     * @brief 低延时可靠有序消息
     */
    
    kMessageConfigReliableOrdered = 0,
    /**
     * 
     * @brief 超低延时有序消息
     */
    
    kMessageConfigUnreliableOrdered = 1,
    /**
     * 
     * @brief 超低延时无序消息
     */
    
    kMessageConfigUnreliableUnordered = 2
};

/**
 * 
 * @type errorcode
 * @brief 发送消息结果，成功或失败，及失败原因
 */

enum UserMessageSendResult {
    /**
     * 
     * @brief 发送消息成功。
     */
    
    kUserMessageSendResultSuccess = 0,
    /**
     * 
     * @brief 消息发送失败。发送超时。
     */
    
    kUserMessageSendResultTimeout = 1,
    /**
     * 
     * @brief 消息发送失败。连接断开，消息未发出。
     */
    
    kUserMessageSendResultNetworkDisconnected = 2,
    /**
     * 
     * @brief 消息发送失败。找不到接收方。
     */
    
    kUserMessageSendResultNoReceiver = 3,
    /**
     * 
     * @brief 消息发送失败。远端用户没有登录或进房。
     */
    
    kUserMessageSendResultNoRelayPath = 4,
    /**
     * 
     * @brief 消息发送失败。超过 QPS 限制。
     */
    
    kUserMessageSendResultExceedQPS = 5,
    /**
     * 
     * @brief 消息发送失败。应用服务器未收到客户端发送的消息。 <br>
     *        由 `sendServerMessage`/`sendServerBinaryMessage` 触发，通过 `onServerMessageSendResult` 回调。
     */
    
    kUserMessageSendResultE2BSSendFailed = 17,
    /**
     * 
     * @brief 消息发送失败。应用服务器接收到了客户端发送的消息，但响应失败。 <br>
     *        由 `sendServerMessage`/`sendServerBinaryMessage` 触发，通过 `onServerMessageSendResult` 回调。
     */
    
    kUserMessageSendResultE2BSReturnFailed = 18,
    /**
     * 
     * @brief 消息发送失败。消息发送方没有加入房间。
     */
    
    kUserMessageSendResultNotJoin = 100,
    /**
     * 
     * @brief 消息发送失败。连接未完成初始化。
     */
    
    kUserMessageSendResultInit = 101,
    /**
     * 
     * @brief 消息发送失败。没有可用的数据传输通道连接。
     */
    
    kUserMessageSendResultNoConnection = 102,
    /**
     * 
     * @brief 消息发送失败。消息超过最大长度 (64 KB)。
     */
    
    kUserMessageSendResultExceedMaxLength = 103,
    /**
     * 
     * @brief 消息发送失败。接收方用户 ID 为空。
     */
    
    kUserMessageSendResultEmptyUser = 104,
    /**
     * 
     * @brief 消息发送失败。房间外或应用服务器消息发送方没有登录。
     */
    
    kUserMessageSendResultNotLogin = 105,
    /**
     * 
     * @brief 消息发送失败。发送消息给业务方服务器之前没有设置参数。
     */
    
    kUserMessageSendResultServerParamsNotSet = 106,
    /**
     * 
     * @brief 消息发送失败。未知错误。
     */
    
    kUserMessageSendResultUnknown = 1000,
};

/**
 * 
 * @type keytype
 * @brief 房间内广播消息发送结果
 */

enum RoomMessageSendResult {
    /**
     * 
     * @brief 消息发送成功
     */
    
    kRoomMessageSendResultSuccess = 200,
    /**
     * 
     * @brief 发送超时，没有发送
     */
    
    kRoomMessageSendResultTimeout = 1,
    /**
     * 
     * @brief 通道断开，没有发送
     */
    
    kRoomMessageSendResultNetworkDisconnected = 2,
    /**
     * 
     * @brief 超过 QPS 限制
     */
    
    kRoomMessageSendResultExceedQPS = 5,
    /**
     * 
     * @brief 失败，发送方未加入房间
     */
    
    kRoomMessageSendResultNotJoin = 100,
    /**
     * 
     * @brief 失败，没有可用的数据传输通道连接
     */
    
    kRoomMessageSendResultNoConnection = 102,
    /**
     * 
     * @brief 失败，消息超过最大长度，当前为 64KB
     */
    
    kRoomMessageSendResultExceedMaxLength = 103,
    /**
     * 
     * @brief 失败，未知错误
     */
    
    kRoomMessageSendResultUnknown = 1000,
};

/**
 * 
 * @type keytype
 * @brief SDK 与信令服务器连接状态。
 */

enum ConnectionState {
    /**
     * 
     * @brief 连接断开超过 12s，此时 SDK 会尝试自动重连。
     */
    
    kConnectionStateDisconnected = 1,
    /**
     * 
     * @brief 首次请求建立连接，正在连接中。
     */
    
    kConnectionStateConnecting = 2,
    /**
     * 
     * @brief 首次连接成功。
     */
    
    kConnectionStateConnected = 3,
    /**
     * 
     * @brief 涵盖了以下情况： <br>
     *        - 首次连接时，10 秒连接不成功;
     *        - 连接成功后，断连 10 秒。自动重连中。
     */
    
    kConnectionStateReconnecting = 4,
    /**
     * 
     * @brief 连接断开后，重连成功。
     */
    
    kConnectionStateReconnected = 5,
    /**
     * 
     * @brief 处于 `kConnectionStateDisconnected` 状态超过 10 秒，且期间重连未成功。SDK 仍将继续尝试重连。
     */
    
    kConnectionStateLost = 6,
    /**
     * 
     * @brief 连接失败，服务端状态异常。SDK 不会自动重连，请重新进房，或联系技术支持。
     */
    
    kConnectionStateFailed = 7,
};

/**
 * 
 * @type keytype
 * @brief 媒体流网络质量。
 */

enum NetworkQuality {
    /**
     * 
     * @brief 网络质量未知。
     */
    
    kNetworkQualityUnknown = 0,
    /**
     * 
     * @brief 网络质量极好。
     */
    
    kNetworkQualityExcellent,
    /**
     * 
     * @brief 主观感觉和 kNetworkQualityExcellent 差不多，但码率可能略低。
     */
    
    kNetworkQualityGood,
    /**
     * 
     * @brief 主观感受有瑕疵但不影响沟通。
     */
    
    kNetworkQualityPoor,
    /**
     * 
     * @brief 勉强能沟通但不顺畅。
     */
    
    kNetworkQualityBad,
    /**
     * 
     * @brief 网络质量非常差，基本不能沟通。
     */
    
    kNetworkQualityVbad,
    /**
     * 
     * @brief 网络连接断开，无法通话。网络可能由于 12s 内无应答、开启飞行模式、拔掉网线等原因断开。 <br>
     *        更多网络状态信息参见 [连接状态提示](https:
     */
    
    kNetworkQualityDown,
};

/**
 * 
 * @type keytype
 * @brief SDK 网络连接类型。
 */

enum NetworkType {
    /**
     * 
     * @brief 网络连接类型未知。
     */
    
    kNetworkTypeUnknown = -1,
    /**
     * 
     * @brief 网络连接已断开。
     */
    
    kNetworkTypeDisconnected = 0,
    /**
     * 
     * @brief 网络连接类型为 LAN 。
     */
    
    kNetworkTypeLAN = 1,
    /**
     * 
     * @brief 网络连接类型为 Wi-Fi（包含热点）。
     */
    
    kNetworkTypeWIFI = 2,
    /**
     * 
     * @brief 网络连接类型为 2G 移动网络。
     */
    
    kNetworkTypeMobile2G = 3,
    /**
     * 
     * @brief 网络连接类型为 3G 移动网络。
     */
    
    kNetworkTypeMobile3G = 4,
    /**
     * 
     * @brief 网络连接类型为 4G 移动网络。
     */
    
    kNetworkTypeMobile4G = 5,
    /**
     * 
     * @brief 网络连接类型为 5G 移动网络。
     */
    
    kNetworkTypeMobile5G = 6,
};

/**
 * 
 * @type errorcode
 * @brief 回调错误码。
 */

enum RtsErrorCode {
    /**
     * 
     * @brief Token 无效。 <br>
     *        进房时使用的 Token 无效或过期失效。需要用户重新获取 Token，并调用 `updateToken` 方法更新 Token。
     */
    
    kRtsErrorCodeInvalidToken = -1000,
    /**
     * 
     * @brief 加入房间错误。 <br>
     *        进房时发生未知错误导致加入房间失败。需要用户重新加入房间。
     */
    
    kRtsErrorCodeJoinRoom = -1001,
    /**
     * 
     * @brief 相同用户 ID 的用户加入本房间，当前用户被踢出房间
     */
    
    kRtsErrorCodeDuplicateLogin = -1004,
    /**
     * 
     * @brief 服务端调用 OpenAPI 将当前用户踢出房间
     */
    
    kRtsErrorCodeKickedOut = -1006,
    /**
     * 
     * @brief 当调用 `createRTCRoom` ，如果 roomid 非法，会返回 null，并抛出该 error
     */
    
    kRtsRoomErrorCodeRoomIdIllegal = -1007,
    /**
     * 
     * @brief Token 过期。调用 `joinRoom` 使用新的 Token 重新加入房间。
     */
    
    kRtsRoomErrorTokenExpired = -1009,
    /**
     * 
     * @brief 调用 `updateToken` 传入的 Token 无效
     */
    
    kRtsRoomErrorUpdateTokenWithInvalidToken = -1010,
    /**
     * 
     * @brief 服务端调用 OpenAPI 解散房间，所有用户被移出房间。
     */
    
    kRtsErrorCodeRoomDismiss = -1011,
    /**
     * 
     * @brief 通话回路检测已经存在同样 roomId 的房间了
     */
    
    kRtsErrorCodeRoomAlreadyExist = -1013,
    /**
     * 
     * @brief 同一个引擎实例在进入多个房间时设置的用户名必须相同
     */
    
    kRtsErrorCodeUserIDDifferent = -1014,
    /**
     * 
     * @brief 服务端异常状态导致退出房间。 <br>
     *        SDK 与信令服务器断开，并不再自动重连，可联系技术支持。
     */
    
    kRtsErrorCodeAbnormalServerStatus = -1084,
    /**
     * 
     * @hidden for internal use only
     */
    
    kRtsErrorCodeInternalDeadLockNotify = -1111,
    /**
     * 
     * @hidden internal use only
     * @brief 加入房间错误。通过 onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged} 回调。 <br>
     *        进房时, LICENSE 计费账号未使用 LICENSE_AUTHENTICATE SDK，加入房间错误。
     */
    
    kRtsErrorCodeJoinRoomWithoutLicenseAuthenticateSDK = -1012,
    /**
     * 
     * @brief 加入RTS房间错误。当前为受限模式。
     */
    
    kRtsErrorCodeJoinRoomLimitedMode = -1013,
};

/**
 * 
 * @type errorcode
 * @brief 回调警告码。 <br>
 *        警告码说明 SDK 内部遇到问题正在尝试恢复。警告码仅起通知作用。
 */

enum RtsWarningCode {
    /**
     * 
     * @brief 进房失败。 <br>
     *        当你调用初次加入房间或者由于网络状况不佳断网重连时，由于服务器错误导致进房失败。SDK 会自动重试进房。
     */
    
    kRtsWarningCodeJoinRoomFailed = -2001,
    /**
     * 
     * @brief 发布音视频流失败。 <br>
     *        当你在所在房间中发布音视频流时，由于服务器错误导致发布失败。SDK 会自动重试发布。
     */
    
    kRtsWarningCodePublishStreamFailed = -2002,
    /**
     * 
     * @hidden currently not available
     * @brief 函数调用顺序错误。
     */
    
    kRtsWarningCodeInvokeError = -2005,
    /**
     * 
     * @hidden for internal use only
     * @brief 调度异常，服务器返回的媒体服务器地址不可用。
     */
    
    kRtsWarningCodeInvalidExpectMediaServerAddress = -2007,
    /**
     * 
     * @brief 发送自定义广播消息失败，当前你未在房间中。
     */
    
    kRtsWarningCodeSendCustomMessage = -2011,
    /**
     * 
     * @brief 当房间内人数超过 500 人时，停止向房间内已有用户发送 `onUserJoined` 和 `onUserLeave` 回调，并通过广播提示房间内所有用户。
     */
    
    kRtsWarningCodeUserNotifyStop = -2013,


    /**
     * 
     * @brief 新生成的房间已经替换了同样 roomId 的旧房间
     */
    
    kRtsWarningCodeOldRoomBeenReplaced = -2016
};

/**
 * 
 * @hidden
 * @deprecated since 3.50
 * @type keytype
 * @brief 事务检查码 <br>
 *        用户调用 `setBusinessId` 方法设置业务标识参数的返回错误码。
 */

enum BusinessCheckCode {
    /**
     * 
     * @brief 用户已经在房间中。 <br>
     *        业务标识需要在加入房间之前设置，加入后设置无效。
     */
    
    kBusinessCheckCodeAlreadyInRoom = -6001,

    /**
     * 
     * @brief 输入参数非法。 <br>
     *        用户传入的业务标识参数非法，参数合法性参考 `setBusinessId` 方法的参数说明。
     */
    
    kBusinessCheckCodeInputInvalidate = -6002,
};

/**
 * 
 * @type keytype
 * @brief App 使用的 CPU 和内存信息。 <br>
 *        该信息由 SDK 周期性（2s）通过 `onSysStats` 回调通知给用户。
 */

struct SysStats {
    /**
     * 
     * @brief 当前系统 CPU 核数。
     */
    
    unsigned int cpu_cores;
    /**
     * 
     * @brief 当前应用的 CPU 使用率，取值范围为 [0, 1]。
     */
    
    double cpu_app_usage;
    /**
     * 
     * @hidden currently not available
     * @brief 当前系统的 cpu 使用率，取值范围为 [0, 1]。
     */
    
    double cpu_total_usage;
    /**
     * 
     * @brief 当前应用的内存使用量（单位 MB）。
     */
    
    double memory_usage;
    /**
     * 
     * @brief 全量内存（单位 MB）。
     */
    
    unsigned long long full_memory;
    /**
     * 
     * @brief 系统已使用内存（单位 MB）。
     */
    
    unsigned long long total_memory_usage;
    /**
     * 
     * @brief 空闲可分配内存（单位 MB）。
     */
    
    unsigned long long free_memory;
    /**
     * 
     * @brief 当前应用的内存使用率（单位 %）。
     */
    
    double memory_ratio;
    /**
     * 
     * @brief 系统内存使用率（单位 %）。
     */
    
    double total_memory_ratio;
};

/**
 * 
 * @type keytype
 * @brief HTTP/HTTPS 代理状态
 */

enum HttpProxyState {
    /**
     * 
     * @brief HTTP/HTTPS 初始化状态
     */
    
    kHttpProxyStateInit,
    /**
     * 
     * @brief HTTP/HTTPS 连接成功
     */
    
    kHttpProxyStateConnected,
    /**
     * 
     * @brief HTTP/HTTPS 连接失败
     */
    
    kHttpProxyStateError
};

/**
 * 
 * @type keytype
 * @brief SOCKS5 代理连接状态
 */

enum Socks5ProxyState {
    /**
     * 
     * @brief SOCKS5 连接初始化
     */
    
    kSocks5Init,
    /**
     * 
     * @brief SOCKS5 连接成功
     */
    
    kSocks5Connected,
    /**
     * 
     * @brief SOCKS5 连接错误
     */
    
    kSocks5Error,
    /**
     * 
     * @brief TCP 连接失败
     */
    
    kSocks5TcpConnectFail,
    /**
     * 
     * @brief TCP 连接关闭
     */
    
    kSocks5TcpClose,
    /**
     * 
     * @brief SOCKS 协议 TCP 协商阶段失败
     */
    
    kSocks5ProtocolTcpFail,
    /**
     * 
     * @brief UDP 连接失败
     */
    
    kSocks5ProtocolUdpFail,
    /**
     * 
     * @brief 授权失败
     */
    
    kSocks5AuthFail,
    /**
     * 
     * @brief 未知状态
     */
    
    kSocks5Unknown
};

/**
 * 
 * @type keytype
 * @brief 云代理信息
 */

struct CloudProxyInfo {
    /**
     * 
     * @type keytype
     * @brief 云代理服务器 IP
     */
    
    const char* cloud_proxy_ip;
    /**
     * 
     * @type keytype
     * @brief 云代理服务器端口
     */
    
    int cloud_proxy_port;
};

/**
 * 
 * @type keytype
 * @brief 云代理相关信息
 */

struct CloudProxyConfiguration {
    /**
     * 
     * @brief 云代理信息。
     */
    
    CloudProxyInfo* cloud_proxies = nullptr;
    /**
     * 
     * @brief 云代理数量。
     */
    
    int cloud_proxy_count = 0;
};

/**
 * 
 * @type keytype
 * @brief ID 最大长度
 */

const unsigned int MAX_DEVICE_ID_LENGTH = 512;

/**
 * 
 * @type keytype
 * @brief 本地代理的类型。
 */

enum LocalProxyType {
    /**
     * 
     * @brief Socks5 代理。选用此代理服务器，需满足 Socks5 协议标准文档的要求。
     */
    
    kLocalProxyTypeSocks5 = 1,
    /**
     * 
     * @brief Http 隧道代理。
     */
    
    kLocalProxyTypeHttpTunnel = 2
};

/**
 * 
 * @type keytype
 * @brief 本地代理配置详细信息。
 */

struct LocalProxyConfiguration {
    /**
     * 
     * @brief 本地代理类型，参看 LocalProxyType{@link #LocalProxyType}。
     */
    
    LocalProxyType local_proxy_type;
    /**
     * 
     * @type keytype
     * @brief 本地代理服务器 IP。
     */
    
    const char* local_proxy_ip;
    /**
     * 
     * @type keytype
     * @brief 本地代理服务器端口。
     */
    
    int local_proxy_port;
    /**
     * 
     * @type keytype
     * @brief 本地代理用户名。
     */
    
    const char* local_proxy_username;
    /**
     * 
     * @type keytype
     * @brief 本地代理密码。
     */
    
    const char* local_proxy_password;
};

/**
 * 
 * @type keytype
 * @brief 本地代理连接状态。
 */

enum LocalProxyState {
    /**
     * 
     * @brief TCP 代理服务器连接成功。
     */
    
    kLocalProxyStateInited = 0,

    /**
     * 
     * @brief 本地代理连接成功。
     */
    
    kLocalProxyStateConnected = 1,

    /**
     * 
     * @brief 本地代理连接出现错误。
     */
    
    kLocalProxyStateError = 2,
};

/**
 * 
 * @type keytype
 * @brief 本地代理错误。
 */

enum LocalProxyError {
    /**
     * 
     * @brief 本地代理服务器无错误。
     */
    
    kLocalProxyErrorOK = 0,

    /**
     * 
     * @brief 代理服务器回复的版本号不符合 Socks5 协议标准文档的规定，导致 Socks5 代理连接失败。请检查代理服务器是否存在异常。
     */
    
    kLocalProxyErrorSocks5VersionError = 1,

    /**
     * 
     * @brief 代理服务器回复的格式错误不符合 Socks5 协议标准文档的规定，导致 Socks5 代理连接失败。请检查代理服务器是否存在异常。
     */
    
    kLocalProxyErrorSocks5FormatError = 2,

    /**
     * 
     * @brief 代理服务器回复的字段值不符合 Socks5 协议标准文档的规定，导致 Socks5 代理连接失败。请检查代理服务器是否存在异常。
     */
    
    kLocalProxyErrorSocks5InvalidValue = 3,

    /**
     * 
     * @brief 未提供代理服务器的用户名及密码，导致 Socks5 代理连接失败。请重新调用 `setLocalProxy`，在设置本地代理时填入用户名和密码。
     */
    
    kLocalProxyErrorSocks5UserPassNotGiven = 4,

    /**
     * 
     * @brief TCP 关闭，导致 Socks5 代理连接失败。请检查网络或者代理服务器是否存在异常。
     */
    
    kLocalProxyErrorSocks5TcpClosed = 5,

    /**
     * 
     * @brief Http 隧道代理错误。请检查 Http 隧道代理服务器或者网络是否存在异常。
     */
    
    kLocalProxyErrorHttpTunnelFailed = 6,
};

/**
 * 
 * @type keytype
 * @brief 本地日志输出等级。
 */

enum class LocalLogLevel {
    /**
     * 
     * @brief 信息级别。
     */
    
    kInfo     = 0,
    /**
     * 
     * @brief （默认值）警告级别。
     */
    
    kWarning  = 1,
    /**
     * 
     * @brief 错误级别。
     */
    
    kError    = 2,
    /**
     * 
     * @brief 关闭日志。
     */
    
    kNone     = 3
};
 
/**
 * 
 * @type keytype
 * @brief 本地日志参数。
 */

struct LogConfig {
    /**
     * 
     * @brief 日志存储路径，必填。
     */
    
    const char* log_path = nullptr;
    /**
     * 
     * @brief 日志等级，参看 LocalLogLevel{@link #LocalLogLevel}，默认为警告级别，选填。
     */
    
    LocalLogLevel log_level = LocalLogLevel::kWarning;

    /**
     * 
     * @brief 日志文件最大占用的总空间，单位为 MB，选填。取值范围为 1～100 MB，默认值为 10 MB。 <br>
     *        若 `log_file_size` < 1，取 1 MB。若 `log_file_size` > 100，取 100 MB。 <br>
     *        其中，单个日志文件最大为 2 MB： <br>
     *        <ul><li> 若 1 ≤ <code>log_file_size</code> ≤ 2，则会生成一个日志文件。</li><li>若 <code>log_file_size</code> > 2，假设 <code>log_file_size/2</code> 的整数部分为 N，则前 N 个文件，每个文件会写满 2 MB，第 N+1 个文件大小不超过 <code>log_file_size mod 2</code>，否则会删除最老的文件，以此类推。</li></ul>
     */
    
    uint32_t log_file_size = 10;

    /**
     * 
     * @brief 日志文件名前缀，选填。该字符串必须符合正则表达式：[a-zA-Z0-9_@\-\.]{1,128}。 <br>
     *        最终的日志文件名为`前缀 + "_" + 文件创建时间 + "_rtclog".log`，如 `logPrefix_2023-05-25_172324_rtclog.log`。
     */
    
    const char* log_filename_prefix = nullptr;
};

/**
 * 
 * @type keytype
 * @brief 创建引擎参数
 */

struct EngineConfig {
    /**
     * 
     * @brief 每个应用的唯一标识符。只有使用相同的 app_id 生成的实例，才能够进行音视频通信。
     */
    
    const char* app_id = nullptr;

    /**
     * 
     * @brief 如需使用请联系技术支持人员。
     */
    
    const char* parameters = nullptr;

    /**
     * 
     * @brief 自定义插件动态库加载目录，目前仅 Android 平台支持。
     */
    
    const char* native_load_path = nullptr;

    /**
     * 
     * @brief 用于存储 Android 应用程序的上下文（Application Context），仅 Android 平台支持。
     */
    
    void* j_application_context = nullptr;
    /**
     * 
     * @brief 是否是游戏场景。
     */
    
    bool is_game_scene = false;
};

}  
