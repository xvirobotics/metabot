/*
 * Copyright (c) 2024 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room Interface
 */

#pragma once

#include "rtc/bytertc_video_effect_interface.h"  
#include "rtc/bytertc_defines.h"
#include "bytertc_room_event_handler.h"
#include "rtc/bytertc_range_audio_interface.h"
#include "rtc/bytertc_spatial_audio_interface.h"
#include "rtc/bytertc_panoramic_video_interface.h"
namespace bytertc {
/**
 * 
 * @type api
 * @brief 房间接口
 */

class IGameRoom {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IGameRoom() {
    }
    /**
     * 
     * @type api
     * @brief 退出并销毁调用 createGameRoom{@link #IRTCEngine#createGameRoom} 所创建的游戏房间实例。
     * @list 房间管理
     */
    
    virtual void destroy() = 0;
    /**
     * 
     * @type api
     * @brief 加入游戏房间。 <br>
     *        调用 createGameRoom{@link #IRTCEngine#createGameRoom} 创建房间实例后，调用此方法加入房间，同房间内其他用户进行音频通话。
     * @param token 动态密钥，用于对登录用户进行鉴权验证。 <br>
     *        进入房间需要携带 Token。测试时可使用控制台生成临时 Token，正式上线需要使用密钥 SDK 在您的服务端生成并下发 Token。Token 有效期及生成方式参看[使用 Token 完成鉴权](70121)。 <br>
     *       - 使用不同 App ID 的 App 是不能互通的。
     *       - 请务必保证生成 Token 使用的 App ID 和创建引擎时使用的 App ID 相同，否则会导致加入房间失败。
     * @param user_info 用户信息，参看 UserInfo{@link #UserInfo}。
     * @return
     *        -  0: 成功。触发以下回调：
     *          - 本端收到房间状态通知 onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged} 回调。
     *          - 如果本端用户为可见用户，房间内其他用户收到 onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} 回调通知。
     *        - -1：room_id/user_info.uid 包含了无效的参数。
     *        - -2：已经在房间内。接口调用成功后，只要收到返回值为 0 ，且未调用 leaveRoom{@link #IGameRoom#leaveRoom} 成功，则再次调用进房接口时，无论填写的房间 ID 和用户 ID 是否重复，均触发此返回值。
     *        调用失败时，具体失败原因会通过 onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged} 回调告知。
     * @note
     *       - 同一个 App ID 的同一个房间内，每个用户的用户 ID 必须是唯一的。如果两个用户的用户 ID 相同，则后进房的用户会将先进房的用户踢出房间，并且先进房的用户会收到 onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged} 回调通知，错误类型详见 ErrorCode{@link #ErrorCode} 中的 kErrorCodeDuplicateLogin。
     *       - 用户加入房间成功后，在本地网络状况不佳的情况下，SDK 可能会与服务器失去连接，并触发 onConnectionStateChanged{@link #IRTCEngineEventHandler#onConnectionStateChanged} 回调。此时 SDK 会自动重试，直到成功重连。重连成功后，本地会收到 onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged} 回调通知。
     * @list 房间管理
     */
    
    virtual int joinRoom(const char* token, const UserInfo& user_info) = 0;
    /**
     * 
     * @type api
     * @brief 离开游戏房间。 <br>
     *        调用此方法结束通话过程，并释放所有通话相关的资源。
     * @return
     *        - 0: 调用成功。如果用户是房间内可见用户，远端用户收到 onUserLeave{@link #IRTSRoomEventHandler#onUserLeave} 回调通知。
     *        - < 0: 调用失败，参看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明。
     * @note
     *       - 加入房间后，必须调用此方法结束通话，否则无法开始下一次通话。
     *       - 此方法是异步操作，调用返回时并没有真正退出房间。真正退出房间后，本地会收到 onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged} 回调通知。你必须在收到 onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged} 回调后，再销毁房间或引擎，或调用 joinRoom{@link #IGameRoom#joinRoom} 再次加入房间。
     * @list 房间管理
     */
    
    virtual int leaveRoom() = 0;
    /**
     * 
     * @type api
     * @brief 更新游戏房间 Token。 <br>
     *        收到 onTokenWillExpire{@link #IRTCRoomEventHandler#onTokenWillExpire}，onPublishPrivilegeTokenWillExpire{@link #IRTCRoomEventHandler#onPublishPrivilegeTokenWillExpire}，或 onSubscribePrivilegeTokenWillExpire{@link #IRTCRoomEventHandler#onSubscribePrivilegeTokenWillExpire} 时，你必须重新获取 Token，并调用此方法更新 Token，以保证通话的正常进行。
     * @param token 重新获取的有效 Token。 <br>
     *        如果 Token 无效，你会收到 onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged}，错误码是 `-1010`。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note 请勿同时调用 updateToken{@link #IGameRoom#updateToken} 和 joinRoom{@link #IGameRoom#joinRoom} 方法更新 Token。若因 Token 过期或无效导致加入房间失败或已被移出房间，你应该在获取新的有效 Token 后调用 joinRoom{@link #IGameRoom#joinRoom} 重新加入房间。
     * @list 房间管理
     */
    
    virtual int updateToken(const char* token) = 0;
    /**
     * 
     * @type api
     * @brief 通过设置 IGameRoom{@link #IGameRoom} 对象的事件句柄，监听此对象对应的回调事件。
     * @param room_event_handler 参见 IRTCRoomEventHandler{@link #IRTCRoomEventHandler}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @list 房间管理
     */
    
    virtual int setRTCRoomEventHandler(IRTCRoomEventHandler* room_event_handler) = 0;
    /**
     * 
     * @type api
     * @brief 获取游戏房间范围语音接口实例。
     * @return 方法调用结果： <br>
     *        - IRangeAudio：成功，返回一个 IRangeAudio{@link #IRangeAudio} 实例。
     *        - nullptr：失败，当前 SDK 不支持范围语音功能。
     * @note 首次调用该方法须在创建房间后、加入房间前。范围语音相关 API 和调用时序详见[范围语音](https:
     * @list 高级功能
     */
    
    virtual IRangeAudio* getRangeAudio() = 0;
    /**
     * 
     * @type api
     * @brief 开始讲话或者停止讲话时调用，调用该接口启动或停止音频采集。同房间其他用户会收到
     * 回调 OnAudioDeviceStateChanged{@link #IRTCEngineEventHandler#OnAudioDeviceStateChanged}
     * @param [in] enable
     *             true 表示开启采集
     *             false 表示关闭采集，默认设置
     * @return
     *        0:  表示参数检查通过，不代表打开麦克风会成功，比如房间不存在
     *        -2: 传入的room_id为空导致失败
     * @note 不可与 EnableAudioSend{@link #IGameRoom#EnableAudioSend} 同时调用。
     * @list 音频管理
     */
    
    virtual int enableMicrophone(bool enable) = 0;
    /**
     * 
     * @type api
     * @brief 开启或关闭扬声器。
     * @param enable 是否开启声器：<br>
     *               - true：开启扬声器，订阅所有远端用户的音频流。
     *               - false：默认设置。关闭扬声器，取消订阅所有远端用户的音频流。
     * @return
     *        - 0：接口调用成功。
     *        - -3：接口调用失败。没有加入房间。
     * @list 音频管理 
     */
    
    virtual int enableSpeakerphone(bool enable) = 0;
    /**
     * 
     * @type api
     * @brief 开始或停止发送音频流。调用此接口不影响音频采集。同房间其他用户会收到相应的回调。
     * @param enable 是否发送音频流：<br>
     *               - true：发送音频流。
     *               - false：默认设置。停止发送音频流（不会关闭麦克风）。
     * @return
     *        0:  表示参数检查通过，不代表打开麦克风会成功，比如房间不存在
     *        -2: 传入的room_id为空导致失败
     * @note 不可与 EnableMicrophone{@link #IGameRoom#EnableMicrophone} 同时调用。
     * @list 音频管理
     */
    
    virtual int enableAudioSend(bool enable) = 0;
    /**
     * 
     * @type api
     * @brief 是否接收某个特定用户的音频流。关闭声音接收不会影响扬声器或其他音频输出设备的状态。
     * @param user_id 用户 ID，最大长度为128字节的非空字符串。支持的字符集范围为: <br>
     *            1. 26个大写字母 A ~ Z<br>
     *            2. 26个小写字母 a ~ z<br>
     *            3. 10个数字 0 ~ 9<br>
     *            4. 下划线"_", at符"@", 减号"-"
     * @param enable 是否接收指定用户的音频流：<br>
     *               - true：接收该用户的音频流。即允许该用户的音频数据被传递到本地设备并播放。
     *               - false：默认设置，不接收该用户的音频流，即不播放该用户的声音。但不会关闭扬声器，扬声器仍可用于其他音频输出。
     * @return
     *        - 0：接口调用成功。
     *        - -2：传入的用户 ID 为空字符串。
     * @list 音频管理
     */
    
    virtual int enableAudioReceive(const char* user_id, bool enable) = 0;
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
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 假设某远端用户 A 始终在被调节的目标用户范围内， <br>
     *        - 该方法与 setRemoteAudioPlaybackVolume{@link #IRTCEngine#setRemoteAudioPlaybackVolume} 互斥，最新调用的任一方法设置的音量将覆盖此前已设置的音量，效果不叠加；
     *        - 当该方法与 setPlaybackVolume{@link #IRTCEngine#setPlaybackVolume} 方法共同使用时，本地收听用户 A 的音量将为两次设置的音量效果的叠加。
     * @list 音频管理
     */
    
    virtual int setRemoteRoomAudioPlaybackVolume(int volume) = 0;
};

} 