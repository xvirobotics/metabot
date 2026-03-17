/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Interface Lite
 */

#pragma once

#include "bytertc_room.h"

namespace bytertc {

class IRTCRoomEventHandlerEx;
/**
 * 
 * @type api
 * @brief IRTCRoomEx Class
 */
class IRTCRoomEx : public IRTCRoom {
public:
    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 在当前所在房间内发布媒体流
     * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
     * @param type 媒体流类型，用于指定发布音频/视频，参看 MediaStreamType{@link #MediaStreamType}
     * @return  
     *        + 0: 调用成功。<br>
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 
     *        + 如果你已经在用户进房时通过调用 joinRoom{@link #IRTCRoom#joinRoom} 成功选择了自动发布，则无需再调用本接口。<br>
     *        + 调用 setUserVisibility{@link #IRTCRoom#setUserVisibility} 方法将自身设置为不可见后无法调用该方法，需将自身切换至可见后方可调用该方法发布摄像头音视频流。 <br>
     *        + 如果你需要向多个房间发布流，调用 startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms}。  <br>
     *        + 调用此方法后，房间中的所有远端用户会收到 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream} 回调通知，其中成功收到了音频流的远端用户会收到 onFirstRemoteAudioFrame{@link #IRTCEngineEventHandler#onFirstRemoteAudioFrame} 回调，订阅了视频流的远端用户会收到 onFirstRemoteVideoFrameDecoded{@link #IRTCEngineEventHandler#onFirstRemoteVideoFrameDecoded} 回调。<br>
     *        + 调用 unpublishStream{@link #IRTCRoom#unpublishStream} 取消发布。
     */
    virtual int publishStream(StreamIndex stream_index, MediaStreamType type) = 0;
    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 停止将本地摄像头/麦克风采集的媒体流发布到当前所在房间中
     * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
     * @param type 媒体流类型，用于指定停止发布音频/视频，参看 MediaStreamType{@link #MediaStreamType}
     * @return  
     *        + 0: 调用成功。<br>
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note  
     *        + 调用 publishStream{@link #IRTCRoomEx#publishStream} 手动发布摄像头音视频流后，你需调用此接口停止发布。<br>
     *        + 调用此方法停止发布音视频流后，房间中的其他用户将会收到 onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream} 回调通知。
     */
    virtual int unpublishStream(StreamIndex stream_index, MediaStreamType type) = 0;
    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 订阅房间内指定的媒体流，或更新对指定媒体流的订阅选项
     * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
     * @param user_id 指定订阅的远端发布音视频流的用户 ID。
     * @param type 媒体流类型，用于指定订阅音频/视频。参看 MediaStreamType{@link #MediaStreamType}。
     * @return 方法调用结果：<br>
     *        + 0：成功；<br>
     *        + <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note  
     *        + 若当前用户在调用本接口时已经订阅该远端用户（手动订阅或自动订阅），则将根据本次传入的参数，更新订阅配置。<br>
     *        + 你必须先通过 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream} 回调获取当前房间里的远端摄像头音视频流信息，然后调用本方法按需订阅。  <br>
     *        + 调用该方法后，你会收到 onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed} 通知方法调用结果。  <br>
     *        + 成功订阅远端用户的媒体流后，订阅关系将持续到调用 unsubscribeStream{@link #IRTCRoom#unsubscribeStream} 取消订阅或本端用户退房。 <br>
     *        + 关于其他调用异常，你会收到 onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} 回调通知，具体异常原因参看 ErrorCode{@link #ErrorCode}。
     */
    virtual int subscribeStream(const char *user_id, StreamIndex stream_index, MediaStreamType type) = 0;
    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 取消订阅房间内指定的媒体流。  <br>
     *        该方法对自动订阅和手动订阅模式均适用。
     * @param user_id 指定取消订阅的远端发布音视频流的用户 ID。
     * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
     * @param type 媒体流类型，用于指定取消订阅音频/视频。参看 MediaStreamType{@link #MediaStreamType}。
     * @return 方法调用结果：<br>
     *        + 0：成功；<br>
     *        + <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note  
     *        + 调用该方法后，你会收到 onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed} 通知方法调用结果。  <br>
     *        + 关于其他调用异常，你会收到 onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} 回调通知，具体失败原因参看 ErrorCode{@link #ErrorCode}。
     */
    virtual int unsubscribeStream(const char *user_id, StreamIndex stream_index, MediaStreamType type) = 0;
    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 通过设置 IRTCRoomEx{@link #IRTCRoomEx} 对象的事件句柄，监听此对象对应的回调事件。
     * @param room_event_handler 参见 IRTCRoomEventHandlerEx{@link #IRTCRoomEventHandlerEx}
     * @return  
     *        + 0: 调用成功。<br>
     *        + < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 设置期望订阅的远端视频流的参数。
     * @param userId 期望配置订阅参数的远端视频流发布用户的 ID。
     * @param stream_index 流索引，参看 StreamIndex{@link #StreamIndex}。
     * @param remoteVideoConfig 期望配置的远端视频流参数，参看 ByteRTCRemoteVideoConfig{@link #ByteRTCRemoteVideoConfig}。
     * @return 方法调用结果： <br>
     *        + 0：成功；<br>
     *        + !0：失败。
     * @note 
     *        + 若使用 342 及以前版本的 SDK，调用该方法前请联系技术支持人员开启按需订阅功能。  <br>
     *        + 该方法仅在发布端调用 setLocalSimulcastMode:{@link #IRTCEngine#setLocalSimulcastMode:} 开启了发送多路视频流的情况下生效，此时订阅端将收到来自发布端与期望设置的参数最相近的一路流；否则订阅端只会收到一路参数为分辨率 640px × 360px、帧率 15fps 的视频流。  <br>
     *        + 若发布端开启了推送多路流功能，但订阅端不对流参数进行设置，则默认接受发送端设置的分辨率最大的一路视频流。  <br>
     *        + 该方法需在进房后调用，若想进房前设置，你需调用 joinRoom:userInfo:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:roomConfig:}，并对 `roomConfig` 中的 `remoteVideoConfig` 进行设置。  <br>
     *        + SDK 会根据发布端和所有订阅端的设置灵活调整视频流的参数，具体调整策略详见[推送多路流](https:
     */
    
};

}  