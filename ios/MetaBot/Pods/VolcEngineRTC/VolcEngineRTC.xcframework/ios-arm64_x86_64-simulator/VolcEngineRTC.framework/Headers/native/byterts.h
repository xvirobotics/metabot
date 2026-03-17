/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Interface
 */

#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <memory>
#include "rtc/bytertc_rts_defines.h"
#include "byterts_event_handler.h"
#include "byterts_room.h"

namespace byterts {

class IRTSRoomEventHandler;
/**
 * 
 * @type api
 * @brief 引擎API
 *      IRtcEngine用IRTSEventHandler 注册接口类方法向应用程序发送回调通知
 */

class IRTS {
public:

    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IRTS() {
    }

    /**
     * 
     * @type api
     * @region 引擎管理
     * @brief 设置业务标识参数  <br>
     *        可通过 businessId 区分不同的业务场景。businessId 由客户自定义，相当于一个“标签”，可以分担和细化现在 AppId 的逻辑划分的功能，但不需要鉴权。
     * @param business_id  <br>
     *        用户设置的自己的 businessId 值
     *        businessId 只是一个标签，颗粒度需要用户自定义。
     * @return  <br>
     *        + 0： 成功  <br>
     *        + < 0： 失败，具体原因参照 BusinessCheckCode{@link #BusinessCheckCode} 。  <br>
     *        + -6001： 用户已经在房间中。  <br>
     *        + -6002： 输入非法，合法字符包括所有小写字母、大写字母和数字，除此外还包括四个独立字符，分别是：英文句号，短横线，下划线和 @ 。
     * @note  <br>
     *        + 需要在进房之前调用，进房之后调用该方法无效。
     */
    
    virtual int setBusinessId(const char* business_id) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 登录 RTS 服务器。
     *        必须先登录，才能发送点对点消息和向应用服务器发送消息<br>
     *        在调用本接口登录后，如果想要登出，需要调用 logout{@link #IRTS#logout}。  <br>
     * @param [in]token  用户登录必须携带的 Token，用于鉴权验证。
     *               测试时可使用[控制台](https:
     *               正式上线需要使用密钥 SDK 在你的服务端生成并下发 Token，`roomId` 置空，Token 有效期及生成方式参看[使用 Token 完成鉴权](70121)。
     * @param uid  <br>
     *        用户 ID  <br>
     *        用户 ID 在 appid 的维度下是唯一的。
     * @return <br>
     *        + `0`：成功<br>
     *        + `-1`：失败。无效参数<br>
     *        + `-2`：无效调用。用户已经登录。成功登录后再次调用本接口将收到此返回值 <br>
     * @note 本地用户调用此方法登录后，会收到 onLoginResult{@link #IRTSEventHandler#onLoginResult} 回调通知登录结果，远端用户不会收到通知。
     */
    
    virtual int login(const char* token, const char* uid) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 调用本接口登出后，无法调用房间外消息以及端到服务器消息相关的方法或收到相关回调。
     * @return  <br>
     *        + 0: 调用成功。
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note  <br>
     *       + 调用本接口登出前，必须先调用 login{@link #IRTS#login} 登录。  <br>
     *       + 本地用户调用此方法登出后，会收到 onLogout{@link #IRTSEventHandler#onLogout} 回调通知结果，远端用户不会收到通知。
     */
    
    virtual int logout() = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 更新用户用于登录的 Token  <br>
     *        Token 有一定的有效期，当 Token 过期时，需调用此方法更新登录的 Token 信息。  <br>
     *        调用 login{@link #IRTS#login} 方法登录时，如果使用了过期的 Token 将导致登录失败，并会收到 onLoginResult{@link #IRTSEventHandler#onLoginResult} 回调通知，错误码为 kLoginErrorCodeInvalidToken。此时需要重新获取 Token，并调用此方法更新 Token。
     * @param token  <br>
     *        更新的动态密钥
     * @return  <br>
     *        + 0: 调用成功。
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note  <br>
     *       + 如果 Token 无效导致登录失败，则调用此方法更新 Token 后，SDK 会自动重新登录，而用户不需要自己调用 login{@link #IRTS#login} 方法。  <br>
     *       + Token 过期时，如果已经成功登录，则不会受到影响。Token 过期的错误会在下一次使用过期 Token 登录时，或因本地网络状况不佳导致断网重新登录时通知给用户。
     */
    
    virtual int updateLoginToken(const char* token) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 设置应用服务器参数  <br>
     *        客户端调用 sendServerMessage{@link #IRTS#sendServerMessage} 或 sendServerBinaryMessage{@link #IRTS#sendServerBinaryMessage} 发送消息给业务服务器之前，必须设置有效签名和业务服务器地址。
     * @param signature 动态签名，应用服务器可使用该签名验证消息来源。<br>
     *        签名需自行定义，可传入任意非空字符串，建议将 uid 等信息编码为签名。<br>
     *        设置的签名会以 post 形式发送至通过本方法中 url 参数设置的应用服务器地址。
     * @param url 应用服务器的地址
     * @return  <br>
     *        + 0: 调用成功。
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note  <br>
     *       + 用户必须调用 login{@link #IRTS#login} 登录后，才能调用本接口。  <br>
     *       + 调用本接口后，SDK 会使用 onServerParamsSetResult{@link #IRTSEventHandler#onServerParamsSetResult} 返回相应结果。
     */
    
    virtual int setServerParams(const char* signature, const char* url) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 查询对端用户或本端用户的登录状态
     * @param peer_user_id  <br>
     *        需要查询的用户 ID
     * @return  <br>
     *        + 0: 调用成功。
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note  <br>
     *       + 必须调用 login{@link #IRTS#login} 登录后，才能调用本接口。  <br>
     *       + 调用本接口后，SDK 会使用 onGetPeerOnlineStatus{@link #IRTSEventHandler#onGetPeerOnlineStatus} 回调通知查询结果。  <br>
     *       + 在发送房间外消息之前，用户可以通过本接口了解对端用户是否登录，从而决定是否发送消息。也可以通过本接口查询自己查看自己的登录状态。
     */
    
    virtual int getPeerOnlineStatus(const char* peer_user_id) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 给房间外指定的用户发送文本消息（P2P）
     * @param uid  <br>
     *        消息接收用户的 ID
     * @param message  <br>
     *        发送的文本消息内容  <br>
     *        消息不超过 62KB。
     * @param config 消息类型，参看 MessageConfig{@link #MessageConfig}。
     * @return  <br>
     *        + >0：发送成功，返回这次发送消息的编号，从 1 开始递增  <br>
     *        + -1：发送失败，RtcEngine 实例未创建  <br>
     *        + -2：发送失败，uid 为空
     * @note  <br>
     *       + 在发送点对点文本消息，必须先调用 login{@link #IRTS#login} 完成登录。  <br>
     *       + 用户调用本接口发送文本信息后，会收到一次 onMessageSendResult{@link #IRTSEventHandler#onMessageSendResult} 回调，得知消息是否成功发送。  <br>
     *       + 若文本消息发送成功，则 uid 所指定的用户会通过 onMessageReceived{@link #IRTSEventHandler#onMessageReceived} 回调收到该消息。
     */
    
    virtual int64_t sendMessage(const char* uid, const char* message, bytertc::MessageConfig config) = 0;
    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 给房间外指定的用户发送二进制消息（P2P）
     * @param uid  <br>
     *        消息接收用户的 ID
     * @param length <br>
     *        二进制字符串的长度
     * @param message  <br>
     *        发送的二进制消息内容  <br>
     *        消息不超过 64KB。
     * @param config 消息类型，参看 MessageConfig{@link #MessageConfig}。
     * @return  <br>
     *        + >0：发送成功，返回这次发送消息的编号，从 1 开始递增  <br>
     *        + -1：发送失败，RtcEngine 实例未创建  <br>
     *        + -2：发送失败，uid 为空
     * @note  <br>
     *       + 在发送点对点二进制消息前，必须先调用 login{@link #IRTS#login} 完成登录。  <br>
     *       + 用户调用本接口发送二进制消息后，会收到一次 onMessageSendResult{@link #IRTSEventHandler#onMessageSendResult} 回调，通知消息是否发送成功。  <br>
     *       + 若二进制消息发送成功，则 uid 所指定的用户会通过 onBinaryMessageReceived{@link #IRTSEventHandler#onBinaryMessageReceived} 回调收到该条消息。
     */
    
    virtual int64_t sendBinaryMessage(const char* uid, int length, const uint8_t* message, bytertc::MessageConfig config) = 0;
    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 客户端给应用服务器发送文本消息（P2Server）
     * @param message  <br>
     *        发送的文本消息内容  <br>
     *        消息不超过 62KB。
     * @return  <br>
     *        + >0：发送成功，返回这次发送消息的编号，从 1 开始递增  <br>
     *        + -1：发送失败，RtcEngine 实例未创建
     * @note  <br>
     *       + 在向业务服务器发送文本消息前，必须先调用 login{@link #IRTS#login} 完成登录，随后调用 setServerParams{@link #IRTS#setServerParams} 设置业务服务器。  <br>
     *       + 调用本接口后，会收到一次 onServerMessageSendResult{@link #IRTSEventHandler#onServerMessageSendResult} 回调，通知消息发送方是否发送成功。  <br>
     *       + 若文本消息发送成功，则之前调用 setServerParams{@link #IRTS#setServerParams} 设置的业务服务器会收到该条消息。
     */
    
    virtual int64_t sendServerMessage(const char* message) = 0;
    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 客户端给应用服务器发送二进制消息（P2Server）
     * @param length <br>
     *        二进制字符串的长度
     * @param message  <br>
     *        发送的二进制消息内容  <br>
     *        消息不超过 64KB。
     * @return  <br>
     *        + >0：发送成功，返回这次发送消息的编号，从 1 开始递增  <br>
     *        + -1：发送失败，RtcEngine 实例未创建
     * @note  <br>
     *       + 在向业务服务器发送二进制消息前，必须先调用 login{@link #IRTS#login} 完成登录，随后调用 setServerParams{@link #IRTS#setServerParams} 设置业务服务器。  <br>
     *       + 调用本接口后，会收到一次 onServerMessageSendResult{@link #IRTSEventHandler#onServerMessageSendResult} 回调，通知消息发送方发送成功或失败。  <br>
     *       + 若二进制消息发送成功，则之前调用 setServerParams{@link #IRTS#setServerParams} 设置的业务服务器会收到该条消息。
     */
    
    virtual int64_t sendServerBinaryMessage(int length, const uint8_t* message) = 0;

    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 创建房间实例。
     *        调用此方法仅返回一个房间实例，你仍需调用 joinRoom{@link #IRTCRoom#joinRoom} 才能真正地创建/加入房间。
     *        多次调用此方法以创建多个 IRTSRoom{@link #IRTSRoom} 实例。分别调用各 IRTSRoom 实例中的 joinRoom{@link #IRTCRoom#joinRoom} 方法，同时加入多个房间。
     *        多房间模式下，用户可以同时订阅各房间的音视频流。
     * @param room_id 标识通话房间的房间 ID。该字符串符合正则表达式：`[a-zA-Z0-9_@\-\.]{1,128}`。
     * @return 创建的 IRTSRoom{@link #IRTSRoom} 房间实例。
     * @note <br>
     *        + 如果需要加入的房间已存在，你仍需先调用本方法来获取 IRTSRoom 实例，再调用 joinRoom{@link #IRTCRoom#joinRoom} 加入房间。
     *        + 多房间模式下，创建多个房间实例需要使用不同的 room_id，否则会导致创建房间失败。
     */
    
    virtual IRTSRoom* createRoom(const char* room_id) = 0;

    /**
     * 
     * @type api
     * @region 引擎管理
     * @brief 设置运行时的参数
     * @param json_string  json 序列化之后的字符串
     * @return  <br>
     *        + 0: 调用成功。
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     */
    
    virtual int setRuntimeParameters(const char * json_string) = 0;

    /**
     * 
     * @hidden(macOS, Windows, Linux)
     * @type api
     * @brief 启用蜂窝网络辅助增强，改善通话质量。
     * @param enhance 是否开启。默认不开启。
     * @return 方法调用结果：  <br>
     *        + 0: 成功。<br>
     *        + -1：失败，内部错误。 <br>
     */
    
    virtual int setCellularEnhancement(bool enhance) = 0;

    /**
     * 
     * @type api
     * @region 代理
     * @brief 设置本地代理
     * @param configurations 本地代理配置参数。参看 LocalProxyConfiguration{@link #LocalProxyConfiguration}。 <br>
     *        你可以根据自己的需要选择同时设置 Http 隧道 和 Socks5 两类代理，或者单独设置其中一类代理。如果你同时设置了 Http 隧道 和 Socks5 两类代理，此时，媒体和信令采用 Socks5 代理， Http 请求采用 Http 隧道代理；如果只设置 Http 隧道 或 Socks5 一类代理，媒体、信令和 Http 请求均采用已设置的代理。 <br>
     *        调用此接口设置本地代理后，若想清空当前已有的代理设置，可再次调用此接口，选择不设置任何代理即可清空。
     * @param configuration_num 本地代理配置参数的数量。
     * @note 要设置本地代理，调用 setLocalProxy{@link #IRTS#setLocalProxy}。
     */
    
    virtual int setLocalProxy(
        const bytertc::LocalProxyConfiguration* configurations,
        int configuration_num) = 0;

    /**
     * 
     * @type api
     * @region 引擎管理
     * @brief 创建 RTCEngine 实例。  <br>
     *        如果当前线程中未创建引擎实例，那么你必须先使用此方法，以使用 RTC 提供的各种音视频能力。  <br>
     *        如果当前线程中已创建了引擎实例，再次调用此方法时，会创建另一个独立的引擎实例。
     * @param config 创建引擎参数配置，详见 EngineConfig{@link #EngineConfig}
     * @param event_handler SDK 回调给应用层的 Callback 对象，详见 IRTSEventHandler{@link #IRTSEventHandler} 。
     * @return  <br>
     *        + IRTS：创建成功。返回一个可用的 IRTS{@link #IRTS} 实例  <br>
     *        + Null：创建失败。
     * @note  <br>
     *        + 如果你在同一个线程中创建了多个引擎实例，这些实例必须分别调用 destroyRTS{@link #destroyRTS} 销毁。
     *        + 在 Linux 平台上创建引擎实例的数量取决于系统的硬件性能。
     */
    
    static BYTERTC_STATIC_API  IRTS* createRTS(const bytertc::EngineConfig& config, IRTSEventHandler* event_handler);

    /**
     * 
     * @type api
     * @region 引擎管理
     * @brief 销毁由 createRTS{@link #createRTS} 创建的 RTCEngine 实例，并释放所有相关资源。
     * @param engine createRTS{@link #createRTS} 时，返回的实例。
     * @note  <br>
     *        + 请确保和需要销毁的 IRTS{@link #IRTS} 实例相关的业务场景全部结束后，才调用此方法。  <br>
     *        + 该方法在调用之后，会销毁所有和此 IRTS{@link #IRTS} 实例相关的内存，并且停止与媒体服务器的任何交互。  <br>
     *        + 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，会导致死锁。同时此方法是耗时操作，不建议在主线程调用本方法，避免主线程阻塞。
     */
    
    static BYTERTC_STATIC_API void destroyRTS();

    /**
     * 
     * @type api
     * @region 引擎管理
     * @brief 获取当前 SDK 版本信息。
     * @return 当前 SDK 版本信息。
     */
    
    static BYTERTC_STATIC_API const char* getSDKVersion();

    /**
     * 
     * @type api
     * @region 引擎管理
     * @brief 配置 SDK 本地日志参数，包括日志级别、存储路径、日志文件最大占用的总空间、日志文件名前缀。
     * @param log_config 本地日志参数，参看 LogConfig{@link #LogConfig}。
     * @return <br>
     *        + 0：成功。
     *        + –1：失败，本方法必须在创建引擎前调用。
     *        + –2：失败，参数填写错误。
     * @note 本方法必须在调用 createRTS{@link #createRTS} 之前调用。
     */
    
    static BYTERTC_STATIC_API int setLogConfig(const bytertc::LogConfig& log_config);
};
}    

