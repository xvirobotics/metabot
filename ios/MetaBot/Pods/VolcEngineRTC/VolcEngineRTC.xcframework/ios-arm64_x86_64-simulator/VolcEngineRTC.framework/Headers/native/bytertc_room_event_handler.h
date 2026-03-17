/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room Event Handler Interface
*/

#pragma once

#include "rtc/bytertc_defines.h"
#include "bytertc_rts_room_event_handler.h"

namespace bytertc {
/**
 * 
 * @type callback
 * @brief RTS房间事件回调接口 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 
 */

class IRTCRoomEventHandler : public IRTSRoomEventHandler {
public:
    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~IRTCRoomEventHandler() {
    }
    /**
     * 
     * @hidden
     * @type callback
     * @brief 房间状态改变回调，加入房间、异常退出房间、发生房间相关的警告或错误时会收到此回调。
     * @param room_id 房间 ID。
     * @param uid 用户 ID。
     * @param state 房间状态码。 <br>
     *              - 0: 加入房间成功。
     *              - 1: 加入房间失败、异常退房、发生房间相关的警告或错误。
     *              - 2: 离开房间。
     * @param reason 房间状态发生变化的原因。参见 RoomStateChangeReason{@link #RoomStateChangeReason}。
     * @list 房间管理
     */
    
    virtual void onRoomStateChangedWithReason(
            const char* room_id, const char* uid, RoomState state, RoomStateChangeReason reason) {
        (void)room_id;
        (void)uid;
        (void)state;
        (void)reason;
    }
    /**
     * 
     * @type callback
     * @valid since 3.60. 自版本 3.60 起，该回调取代 `onStreamStateChanged` 回调，用于报告视频发布状态改变。如果您已升级 SDK 至版本 3.60 或以上，仍在使用 `onStreamStateChanged` 回调，请迁移至该回调。
     * @region 房间管理
     * @brief 视频发布状态改变回调。
     * @param stream_id 流 ID。
     * @param stream_info 流信息。
     * @param state 发布状态码，参看 PublishState{@link #PublishState}。
     * @param reason 本地视频流发布状态改变的具体原因，参看 PublishStateChangeReason{@link #PublishStateChangeReason}。
     * @note 除了本地视频流发布状态改变时触发，本端用户进房时也会收到本回调。
     * @list 音视频传输
     */
    
    virtual void onVideoPublishStateChanged(
            const char* stream_id, const StreamInfo& stream_info, PublishState state, PublishStateChangeReason reason) {
        (void)stream_id;
        (void)stream_info;
        (void)state;
        (void)reason;
    }

    /**
     * 
     * @type callback
     * @valid since 3.60.
     * @region 房间管理
     * @brief 音频发布状态改变回调。
     * @param stream_id 流 ID。
     * @param stream_info 流信息。
     * @param state 发布状态码，参看 PublishState{@link #PublishState}。
     * @param reason 本地音频流发布状态改变的具体原因，参看 PublishStateChangeReason{@link #PublishStateChangeReason}。
     * @note 除了状态改变时触发，本端用户进房时也会收到本回调。      
     * @list 音视频传输
     */
    
    virtual void onAudioPublishStateChanged(
            const char* stream_id, const StreamInfo& stream_info, PublishState state, PublishStateChangeReason reason) {
        (void)stream_id;
        (void)stream_info;
        (void)state;
        (void)reason;
    }

    /**
     * 
     * @type callback
     * @valid since 3.60. 自版本 3.60 起，该回调替换了 `onStreamStateChanged` 和 `onStreamSubscribed` 方法，用于报告视频流订阅状态的改变。如果您已升级到 SDK 版本 3.60 或以上，仍在使用这两个方法，请迁移到该回调。
     * @region 房间管理
     * @brief 视频订阅状态发生改变回调。
     * @param stream_id 流 ID。
     * @param stream_info 流信息。
     * @param state 订阅状态码，参看 SubscribeState{@link #SubscribeState}。
     * @param reason 视频流订阅状态改变的具体原因，参看 SubscribeStateChangeReason{@link #SubscribeStateChangeReason}。
     * @note 当调用 subscribeStreamVideo{@link #IRTCRoom#subscribeStreamVideo} 后会收到该通知。
     * @list 音视频传输
     */
    
    virtual void onVideoSubscribeStateChanged(
            const char* stream_id, const StreamInfo& stream_info, SubscribeState state, SubscribeStateChangeReason reason) {
        (void)stream_id;
        (void)stream_info;
        (void)state;
        (void)reason;
    }

    /**
     * 
     * @type callback
     * @valid since 3.60.
     * @region 房间管理
     * @brief 音频订阅状态发生改变回调。
     * @param stream_id 流 ID。
     * @param stream_info 流信息。
     * @param state 订阅状态码，参看 SubscribeState{@link #SubscribeState}。
     * @param reason 音频订阅状态改变的具体原因，参看 SubscribeStateChangeReason{@link #SubscribeStateChangeReason}。
     * @note 当调用 subscribeStreamAudio{@link #IRTCRoom#subscribeStreamAudio} 后会收到该通知。
     * @list 音视频传输
     */
    
    virtual void onAudioSubscribeStateChanged(
            const char* stream_id, const StreamInfo& stream_info, SubscribeState state, SubscribeStateChangeReason reason) {
        (void)stream_id;
        (void)stream_info;
        (void)state;
        (void)reason;
    }

    /**
     * 
     * @hidden
     * @deprecated since 3.41 and will be deleted in 3.51, use onRoomStateChanged{@link #IRTSRoomEventHandler#onRoomStateChanged} and onStreamStateChanged instead.
     * @type callback
     * @brief SDK 发生警告回调。 <br>
     *        SDK 内部遇到可恢复错误时，在尝试自动恢复的同时，会通过此回调通知用户。此回调事件仅用作通知。
     * @param warn 警告码，参看 WarningCode{@link #WarningCode}。
     * @list 房间管理
     */
    
    BYTERTC_DEPRECATED virtual void onRoomWarning(int warn) {
        (void)warn;
    }
    /**
     * 
     * @type callback
     * @brief 当 SDK 检测到 Token 的进房权限将在 30 秒内过期时，触发该回调。
     *        收到该回调后，你需调用 updateToken{@link #IRTSRoom#updateToken} 更新 Token 进房权限。
     * @note 若未能及时更新 Token 进房权限导致其过期实效： <br>
     *        - 用户此时尝试进房会收到  回调，提示错误码为 `-1000` Token 过期；
     *        - 用户已在房间内则会被移出房间，本地用户会收到  回调，提示错误码为 `-1009` Token 过期，同时远端用户会收到 onUserLeave{@link #IRTSRoomEventHandler#onUserLeave} 回调，提示原因为 `2` Token 进房权限过期。
     * @list 引擎管理
     */
     
    virtual void onTokenWillExpire() {

    }
    /**
     * 
     * @type callback
     * @brief Token 发布权限过期前 30 秒将触发该回调。 <br>
     *        收到该回调后，你需调用 updateToken{@link #IRTSRoom#updateToken} 更新 Token 发布权限。
     * @note  Token 发布权限过期后：
     *        - 已发布流或尝试发布流时，本端会收到 onVideoPublishStateChanged{@link #IRTCRoomEventHandler#onVideoPublishStateChanged}、onAudioPublishStateChanged{@link #IRTCRoomEventHandler#onAudioPublishStateChanged} 回调，提示`kPublishStateChangeReasonNoPublishPermission`，没有发布权限。
     *        - 发布中的流将停止发布。远端用户会收到 onUserPublishStreamVideo{@link #IRTCRoomEventHandler#onUserPublishStreamVideo}、onUserPublishStreamAudio{@link #IRTCRoomEventHandler#onUserPublishStreamAudio} 回调，提示该流已停止发布。
     * @list 引擎管理
     */
    
    virtual void onPublishPrivilegeTokenWillExpire() {

    }
    /**
     * 
     * @type callback
     * @brief Token 订阅权限过期前 30 秒将触发该回调。 <br>
     *        收到该回调后，你需调用 updateToken{@link #IRTSRoom#updateToken} 更新 Token 订阅权限有效期。
     * @note 若收到该回调后未及时更新 Token，Token 订阅权限过期后，尝试新订阅流会失败，已订阅的流会取消订阅，并且会收到 onVideoSubscribeStateChanged{@link #IRTCRoomEventHandler#onVideoSubscribeStateChanged}、onAudioSubscribeStateChanged{@link #IRTCRoomEventHandler#onAudioSubscribeStateChanged} 回调，提示`kSubscribeStateChangeReasonNoSubscribePermission`，没有订阅权限。
     * @list 引擎管理
     */
    
    virtual void onSubscribePrivilegeTokenWillExpire() {

    }
    /**
     * 
     * @type callback
     * @brief 房间内通话统计信息回调。 <br>
     *        用户进房开始通话后，每 2s 收到一次本回调。
     * @param stats 当前 RtcEngine 统计数据，详见 RtcRoomStats{@link #RtcRoomStats}
     * @list 房间管理
     */
    
    virtual void onRoomStats(const RtcRoomStats& stats) {
        (void)stats;
    }
   /**
    * 
    * @hidden for internal use only
    * @type callback
    * @brief 房间事件回调。
    * @param room_id 房间 ID。
    * @param uid 用户 ID。
    * @param state 房间事件状态。详见 RoomEvent{@link #RoomEvent}。
    * @param info 房间封禁时，包含封禁时间。详见 RoomEventInfo{@link #RoomEventInfo}。
    * @list 房间管理
    */
   
    virtual void onRoomEvent(const char* room_id, const char* uid, RoomEvent state, const RoomEventInfo& info) {
        (void)room_id;
        (void)uid;
        (void)state;
        (void)info;
    }
    /**
     * 
     * @hidden(Linux)
     * @type callback
     * @brief 本地流数据统计以及网络质量回调。 <br>
     *        本地用户发布流成功后，SDK 会周期性（2s）的通过此回调事件通知用户发布的流在此次统计周期内的质量统计信息。 <br>
     *        统计信息通过 LocalStreamStats{@link #LocalStreamStats} 类型的回调参数传递给用户，其中包括发送音视频比特率、发送帧率、编码帧率，网络质量等。
     * @param stream_id 流 ID。
     * @param stream_info 流信息。
     * @param stats 当前 RtcEngine 统计数据，详见 LocalStreamStats{@link #LocalStreamStats}。
     * @list 音视频传输
     */
    
    virtual void onLocalStreamStats(const char* stream_id, const bytertc::StreamInfo& stream_info, const LocalStreamStats& stats) {
        (void)stream_id;
        (void)stream_info;
        (void)stats;
    }
    /**
     * 
     * @type callback
     * @brief 本地订阅的远端音/视频流数据统计以及网络质量回调。 <br>
     *        本地用户订阅流成功后，SDK 会周期性（2s）的通过此回调事件通知用户订阅的流在此次统计周期内的质量统计信息，包括：发送音视频比特率、发送帧率、编码帧率，网络质量等。
     * @param stream_id 流 ID。
     * @param stream_info 流信息。
     * @param stats 当前 RtcEngine 统计数据，详见 RemoteStreamStats{@link #RemoteStreamStats}
     * @list 音视频传输
     */
    
    virtual void onRemoteStreamStats(const char* stream_id, const bytertc::StreamInfo& stream_info, const RemoteStreamStats& stats) {
        (void)stream_id;
        (void)stream_info;
        (void)stats;
    }
    /**
     * 
     * @type callback
     * @brief 远端可见用户加入房间，或房内不可见用户切换为可见的回调。 <br>
     *        1. 远端用户调用 setUserVisibility{@link #IRTCRoom#setUserVisibility} 方法将自身设为可见后加入房间时，房间内其他用户将收到该事件。 <br>
     *        2. 远端可见用户断网后重新连入房间时，房间内其他用户将收到该事件。 <br>
     *        3. 房间内隐身远端用户调用 setUserVisibility{@link #IRTCRoom#setUserVisibility} 方法切换至可见时，房间内其他用户将收到该事件。 <br>
     *        4. 新进房用户会收到进房前已在房内的可见用户的进房回调通知。
     * @param user_info 用户信息，详见 UserInfo{@link #UserInfo}
     * @list 房间管理
     */
     
    virtual void onUserJoined(const UserInfo& user_info) {
        (void)user_info;
    }
    /**
     * 
     * @hidden
     * @deprecated since 3.36 and will be deleted in 3.51, use onUserUnpublishStream and onUserUnpublishScreen instead.
     * @type callback
     * @brief 房间内的远端用户停止发布音视频流时，本地用户会收到此回调。
     * @param stream 流的属性。参看 MediaStreamInfo{@link #MediaStreamInfo} 。
     * @param reason 远端流移除的原因。参看 StreamRemoveReason{@link #StreamRemoveReason} 。
     * @list 房间管理
     */
    
    BYTERTC_DEPRECATED virtual void onStreamRemove(const MediaStreamInfo& stream, StreamRemoveReason reason) {
        (void)stream;
    }
    /**
     * 
     * @hidden
     * @deprecated since 3.36 and will be deleted in 3.51, use onUserPublishStream and onUserPublishScreen instead.
     * @type callback
     * @brief 房间内的用户发布新的音视频流时，房间内的其他用户会收到此回调。包括移除后又重新发布的流。
     * @param stream 流属性，参看 MediaStreamInfo{@link #MediaStreamInfo} 。
     * @list 房间管理
     */
    
    BYTERTC_DEPRECATED virtual void onStreamAdd(const MediaStreamInfo& stream) {
        (void)stream;
    }

    /**
     * 
     * @type callback
     * @valid since 3.60. 自版本 3.60 起，该回调替换了 `onUserPublishStream` 、`onUserUnpublishStream` 、`onUserPublishScreen` 和 `onUserUnpublishScreen` 方法。如果您已升级到 SDK 版本 3.60 或以上，且仍在使用这些方法，请迁移到该回调。
     * @region 房间管理
     * @brief 房间内远端用户发布或取消发布视频流的回调。
     * @param stream_id 流 ID。
     * @param stream_info 流信息。
     * @param is_publish 流是否发布。
     *         + `true`：已发布。
     *         + `false`：已取消发布。
     * @note 当房间内的远端用户调用 publishStreamVideo{@link #IRTCRoom#publishStreamVideo} 成功发布采集的视频流时，本地用户会收到该回调，此时本地用户可以自行选择是否调用 subscribeStreamVideo{@link #IRTCRoom#subscribeStreamVideo} 订阅此流。
     * @list 音视频传输
     */
    
    virtual void onUserPublishStreamVideo(const char* stream_id, const StreamInfo& stream_info, bool is_publish){
        (void)stream_id;
        (void)stream_info;
        (void)is_publish;
    }

    /**
     * 
     * @type callback
     * @valid since 3.60. 自 3.60 起，该回调替代了 `onUserPublishStream`、`onUserUnpublishStream`、`onUserPublishScreen` 和 `onUserUnpublishScreen` 方法来实现下述功能。如果你已升级至 3.60 及以上版本 SDK，且还在使用这两个方法，请迁移至该回调。
     * @brief 房间内远端用户发布或取消发布音频流的回调。
     * @param stream_id 流 ID。
     * @param stream_info 流信息。
     * @param is_publish 流是否发布。
     *         + `true`：已发布。
     *         + `false`：已取消发布。
     * @note 当房间内的远端用户调用 publishStreamAudio{@link #IRTCRoom#publishStreamAudio} 成功发布音频流时，本地用户会收到该回调，此时本地用户可以自行选择是否调用 subscribeStreamAudio{@link #IRTCRoom#subscribeStreamAudio} 订阅此流。
     * @list 音视频传输
     * @order 2
     */
    
    virtual void onUserPublishStreamAudio(const char* stream_id, const StreamInfo& stream_info, bool is_publish){
        (void)stream_id;
        (void)stream_info;
        (void)is_publish;
    }

    /**
     * 
     * @hidden for internal use only
     * @type callback
     * @brief 当发布流成功的时候回调该事件。
     * @param user_id 发布流的用户 ID。
     * @param is_screen 该流是否为屏幕流。 <br>
     *             - Ture: 屏幕流。
     *             - False: 非屏幕流。
     * @list 音视频传输
     */
    
    virtual void onStreamPublishSuccess(const char* user_id, bool is_screen) {
        (void)is_screen;
    }

    /**
     * 
     * @hidden for internal use only
     */
      
    virtual void onRoomModeChanged(RtcRoomMode mode) {
        (void)mode;
    }

    /**
     * 
     * @hidden for internal use only
     * @brief callback when the maximum screen share fps is changed
     * @param fps maximum screen share fps
     */
    
    virtual void onMaximumScreenShareFpsUpdated(int fps) {
        (void)fps;
    }

    /**
     * 
     * @hidden for internal use only
     * @brief 最大屏幕共享帧率改变时的回调
     * @param screen_pixels 为了保持帧率而推荐的最大视频宽度×高度的值。
     */
    
    virtual void onMaximumScreenSharePixelsUpdated(int screen_pixels) {
        (void)screen_pixels;
    }
    /**
     * 
     * @type callback
     * @brief 通过调用服务端 BanUserStream/UnbanUserStream 方法禁用/解禁指定房间内指定用户视频流的发送时，触发此回调。
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
    
    virtual void onVideoStreamBanned(const char* uid, bool banned) {
        (void)uid;
        (void)banned;
    }
    /**
     * 
     * @type callback
     * @brief 发布端调用 setMultiDeviceAVSync{@link #IRTCRoom#setMultiDeviceAVSync} 后音视频同步状态发生改变时，会收到此回调。
     * @param state 音视频同步状态，参看 AVSyncState{@link #AVSyncState}。
     * @list 音视频传输
     */
    
    virtual void onAVSyncStateChange(AVSyncState state) {
        (void)state;
    }
    /**
     * 
     * @type callback
     * @brief 通过调用服务端 BanUserStream/UnbanUserStream 方法禁用/解禁指定房间内指定用户音频流的发送时，触发此回调。
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
    
    virtual void onAudioStreamBanned(const char* uid, bool banned) {
        (void)uid;
        (void)banned;
    }
    /**
     * 
     * @type callback
     * @brief 跨房间媒体流转发状态和错误回调
     * @param infos 跨房间媒体流转发目标房间信息数组，详见 ForwardStreamStateInfo{@link #ForwardStreamStateInfo}
     * @param info_count 数组长度，代表目标房间数
     * @list 音视频传输
     */
    
    virtual void onForwardStreamStateChanged(ForwardStreamStateInfo* infos, int info_count) {
    }
    /**
     * 
     * @type callback
     * @brief 跨房间媒体流转发事件回调
     * @param infos 跨房间媒体流转发目标房间事件数组，详见 ForwardStreamEventInfo{@link #ForwardStreamEventInfo}
     * @param info_count 数组长度，代表目标房间数
     * @list 音视频传输
     */
    
    virtual void onForwardStreamEvent(ForwardStreamEventInfo* infos, int info_count) {
    }
    /**
     * 
     * @type callback
     * @brief 加入房间并发布或订阅流后， 以每 2 秒一次的频率，报告本地用户和已订阅的远端用户的上下行网络质量信息。
     * @param local_quality 本端网络质量，详见 NetworkQualityStats{@link #NetworkQualityStats}。
     * @param remote_qualities 已订阅用户的网络质量，详见 NetworkQualityStats{@link #NetworkQualityStats}。
     * @param remote_quality_num `remoteQualities` 数组长度
     * @note 更多通话中的监测接口，详见[通话中质量监测](https:
     * @list 音视频传输
     */
    
    virtual void onNetworkQuality(const NetworkQualityStats& local_quality, const NetworkQualityStats* remote_qualities, int remote_quality_num) {
    }
    /**
     * 
     * @valid since 3.52.
     * @type callback
     * @brief 调用 setRoomExtraInfo{@link #IRTCRoom#setRoomExtraInfo} 设置房间附加信息结果的回调。
     * @param task_id 调用 setRoomExtraInfo 的任务编号。
     * @param result 设置房间附加信息的结果，详见 SetRoomExtraInfoResult{@link #SetRoomExtraInfoResult}
     * @list 房间管理
     */
    
    virtual void onSetRoomExtraInfoResult(int64_t task_id, SetRoomExtraInfoResult result) {
        (void)task_id;
        (void)result;
    }
    /**
     * 
     * @valid since 3.52.
     * @type callback
     * @brief 接收同一房间内，其他用户调用 setRoomExtraInfo{@link #IRTCRoom#setRoomExtraInfo} 设置的房间附加信息的回调。
     * @param key 房间附加信息的键值
     * @param value 房间附加信息的内容
     * @param last_update_user_id 最后更新本条信息的用户 ID。
     * @param last_update_time_ms 最后更新本条信息的 Unix 时间，单位：毫秒。
     * @note 新进房的用户会收到进房前房间内已有的全部附加信息通知。
     * @list 房间管理
     */
    
    virtual void onRoomExtraInfoUpdate(const char*key, const char* value, const char* last_update_user_id, int64_t last_update_time_ms) {
        (void)key;
        (void)value;
        (void)last_update_user_id;
        (void)last_update_time_ms;
    }
    /**
     * 
     * @valid since 3.60.
     * @type callback
     * @brief 接收同一房间内，其他用户调用 setStreamExtraInfo{@link #IRTCRoom#setStreamExtraInfo} 设置的流附加信息的回调。
     * @param stream_id 流 ID
     * @param stream_info 流信息，详见 StreamInfo{@link #StreamInfo}
     * @param extra_info 流附加信息的内容
     * @note 新进房的用户会收到进房前房间内已有的全部附加信息通知。
     * @list 房间管理
     */
    
    virtual void onRoomStreamExtraInfoUpdate(const char* stream_id, const StreamInfo& stream_info, const char* extra_info) {
        (void)stream_id;
        (void)stream_info;
        (void)extra_info;
    }
    /**
     * 
     * @valid since 3.54
     * @type callback
     * @brief 用户调用 setUserVisibility{@link #IRTCRoom#setUserVisibility} 设置用户可见性的回调。
     * @param current_user_visibility 当前用户的可见性。 <br>
     *        - true: 可见，用户可以在房间内发布音视频流，房间中的其他用户将收到用户的行为通知，例如进房、开启视频采集和退房。
     *        - false: 不可见，用户不可以在房间内发布音视频流，房间中的其他用户不会收到用户的行为通知，例如进房、开启视频采集和退房。
     * @param error_code 设置用户可见性错误码，参看 UserVisibilityChangeError{@link #UserVisibilityChangeError}。
     * @list 房间管理
     */
    
    virtual void onUserVisibilityChanged(bool current_user_visibility, UserVisibilityChangeError error_code) {
        (void)current_user_visibility;
        (void)error_code;
    }
    /**
     * 
     * @type callback
     * @brief 字幕状态发生改变回调。 <br>
     *         当用户调用 startSubtitle{@link #IRTCRoom#startSubtitle} 和 stopSubtitle{@link #IRTCRoom#stopSubtitle} 使字幕状态发生改变或出现错误时，触发该回调。
     * @param state 字幕状态。参看 SubtitleState{@link #SubtitleState}。
     * @param error_code 字幕任务错误码。参看 SubtitleErrorCode{@link #SubtitleErrorCode}。
     * @param error_message 与第三方服务有关的错误信息。
     * @list 字幕翻译服务
     */
    
    virtual void onSubtitleStateChanged(SubtitleState state, SubtitleErrorCode error_code, const char* error_message) {
    }
    /**
     * 
     * @type callback
     * @brief 字幕相关内容回调。 <br>
     *         当用户调用 startSubtitle{@link #IRTCRoom#startSubtitle} 后会收到此回调，通知字幕的相关信息。
     * @param subtitles 字幕消息内容。参看 SubtitleMessage{@link #SubtitleMessage}。
     * @param cnt 字幕消息个数。
     * @list 字幕翻译服务
     */
    
    virtual void onSubtitleMessageReceived(const SubtitleMessage* subtitles, int cnt) {
    }

    /**
     * 
     * @type callback
     * @valid since 3.60.
     * @brief 发布端调用 setMultiDeviceAVSync{@link #IRTCRoom#setMultiDeviceAVSync} 后音视频同步状态发生错误时，会收到此回调。
     * @param room_id 房间 ID。
     * @param uid 用户 ID。
     * @param event 音视频同步状态错误，参看 AVSyncEvent{@link #AVSyncEvent}。
     * @list 音视频传输
     */
    
    virtual void onAVSyncEvent(const char* room_id, const char* uid, AVSyncEvent event) {
    }
};

} 
