#pragma once

#include "bytertc_defines.h"

namespace bytertc {
class IRTCRoom;
class IRTCEngine;
class IAmazingEffect;

/** 
 * @type keytype
 * @brief 5.18版本后，飞书会议弱网提示专用，设置订阅视频基线数据用于计算网络等级
 */
struct SubscribeVideoBaseline {
    /** 
     * @brief 网络等级判定为优秀的视频宽/高中小值的基线，-1表示业务不设置计算时忽略此参数
     */
    int32_t acceptable_good_video_pixel_baseline = -1;
    /** 
     * @brief 网络等级判定为优秀的视频帧率基线，-1表示业务不设置计算时忽略此参数
     */
    int32_t acceptable_good_video_fps_baseline = -1;
    /** 
     * @brief 网络等级判定为差视频宽/高中小值的基线，-1表示业务不设置计算时忽略此参数
     */
    int32_t acceptable_bad_video_pixel_baseline = -1;
    /** 
     * @brief 网络等级判定为差的视频帧率基线，-1表示业务不设置计算时忽略此参数
     */
    int32_t acceptable_bad_video_fps_baseline = -1;
    /** 
     * @brief 业务可以接受的最小分辨率宽度，-1表示业务忽略此参数
     */
    int32_t acceptable_min_video_resolution_width = -1;
    /** 
     * @brief 业务可以接受的最小分辨率高度，-1表示业务忽略此参数
     */
    int32_t acceptable_min_video_resolution_height = -1;
    /** 
     * @brief 流的优先级，-1表示业务忽略此参数
     */
    int32_t stream_priority = -1;
};

enum SpecialStreamType {
    kSpecialStreamTypeDocShare = 1,
    kSpecialStreamTypeMCU,
    kSpecialStreamTypeMeetTogether,
    kSpecialStreamTypeIntelligentStream,
};

enum ExternalReportType{
    kExternalReportTypeApi = 0,
    kExternalReportTypeApiCallback = 1,
};
}

namespace bytertc {
    /**
     * 
     * @hidden
     * @brief 推流到子频道channelName
     * > 支持会前、会中设置
     * @param [IRTCRoom*] room 目标room
     * @param [const char*] channelName 子频道
     */
    BYTERTC_API int setPublishChannel(IRTCRoom* room, const char* channelName);
    /**
     * 
     * @hidden
     * @brief 订阅子频道数组里的所有流
     * > 支持会前、会中设置
     * @param [IRTCRoom*] room 目标room
     * @param [const SubChannelsInfo*] channelsInfo 子频道数组
     * @param [int] channelsNum 子频道数组大小
     * @param [bool] enableVolumeRescale 是否开启音频抑制功能
     */
    BYTERTC_API int setSubscribeChannels(IRTCRoom* room, const char* channels[], int channelsNum, bool enable_rescale);
    /**
     * 
     * @hidden
     * @brief 设置是否开启音频抑制
     * > 支持会前、会中设置
     * @param [IRTCRoom*] room 目标room
     * @param [bool] enable 是否抑制
     */
    BYTERTC_API int enableRescaleAudioVolume(IRTCRoom* room, bool enable);
    /**
     * 
     * @hidden
     * @deprecated since 3.58 and will be deleted in 3.60
     * @brief 设置发布的特殊流类型
     * > 支持会前设置
     * @param [IRTCRoom*] room 目标room
     * @param [int] type 流类型
     */
    BYTERTC_API int setPublishSpecialStream(IRTCRoom* room, int type);
    /**
     * 
     * @hidden
     * @brief 设置发布的特殊流类型
     * > 支持会前设置
     * @param [IRTCRoom*] room 目标room
     * @param [int] type 流类型
     */
    BYTERTC_API int setPublishSpecialStreamWithStreamIndex(IRTCRoom* room, StreamIndex streamIndex, int type);
    /**
     * 
     * @hidden
     * @brief 设置订阅的特殊流类型
     * > 支持会前、会中设置
     * @param [IRTCRoom*] room 目标room
     * @param [int[]] types 流类型集合
     * @param [int] count 流类型集合大小
     */
    BYTERTC_API int setSubscribeSpecialStream(IRTCRoom* room, int types[], int count);
    /**
     * 
     * @brief 获取上行带宽状态
     * @param [IRTCRoom*] room 目标room
     */
    BYTERTC_API int getUplinkNetworkBandwidthEstimationStatus(IRTCRoom* room);
    /**
     * 
     * @brief 获取下行带宽状态
     * @param [IRTCRoom*] room 目标room
     */
    BYTERTC_API int getDownlinkNetworkBandwidthEstimationStatus(IRTCRoom* room);

    /**
     * 
     * @hidden
     * @brief 视频发布端设置推送多路流时各路流的参数，包括分辨率、帧率、码率、缩放模式、网络不佳时的回退策略等。
     * @param channel_solutions 通道内的最大分辨率
     * @param solution_num 通道数
     * @param channel_main_solutions 主通道 兼容旧版本
     * @return 0 成功 !0失败
     */
    BYTERTC_API int setVideoEncoderConfig(IRTCEngine* ivideo, const VideoEncoderConfig* channel_solutions,
                                          int solution_num, const VideoEncoderConfig* channel_main_solutions);
    
    /**
     * 
     * @hidden
     * @brief 设置屏幕流多分辨率参数
     * @return 0 成功 !0失败
     */
    BYTERTC_API int setScreenVideoConfigs(IRTCEngine* ivideo);

    /**
     * 
     * @hidden
     * @brief 清空屏幕流多分辨率参数
     * @return 0 成功 !0失败
     */
    BYTERTC_API int resetScreenVideoConfigs(IRTCEngine* ivideo);

    /**
     * 
     * @brief 更新自动订阅选项
     *@param [IRTCRoom*] room 目标room
     *@param [SubscribeMode] sub_mode_audio 音频自动订阅模式
     *@param [SubscribeMode] sub_mode_video 视频自动订阅模式
     */
    BYTERTC_API int enableAutoSubscribe(IRTCRoom* room, SubscribeMode sub_mode_audio, SubscribeMode sub_mode_video);

    /**
     * 
     * @brief 检查音视频流是否取消发布成功
     *@param [IRTCRoom*] room 目标room
     *@param [int] type 需要检查的流类型
     */
    BYTERTC_API bool isStreamUnpublished(IRTCRoom* room, MediaStreamType stream_type);

    /**
     * 
     * @brief 设置弱网降级的基线数据
     *@param [RTCRoom] room 目标room
     *@param [String] userId 订阅用户名
     *@param [boolean] isScreen 是否屏幕流
     *@param [InternalSubscribeVideoBaseline] videoBaseline 订阅基线设置
     */
    BYTERTC_API void setSubscribeBaselineData(IRTCRoom* rtc_room, const char* uid, bool is_screen,
                                             const SubscribeVideoBaseline& video_baseline);

    /**
     * 
     * @hidden
     * @brief 获取用于实时crash分子数据信息
     * @param  [char*] params carsh参数信息的json形式的字符数组
     * @param  [int] size 用于存储params信息的数组大小
     */
    BYTERTC_API int getSessionLaunchParams(char* params, int size);

    /**
     * 
     * @hidden 
     * @brief 获取视频特效接口IAmazingEffect
     *@param [RTCEngine] ivideo 目标RTCVideo
     */
    BYTERTC_API IAmazingEffect* getAmazingEffectInterface(IRTCEngine* ivideo);
    /**
     * 
     * @hidden 
     * @brief 获取EffectSDK版本，用于初始化platformsdk
     */
    BYTERTC_API void getEffectVersion(char* version, int size);
    
    /**
     * 
     * @brief 设置外部 Surface
     * @return 无返回值
     */
    BYTERTC_API void setExternalSurface(IRTCEngine* ivideo, const char* room_id, const char* user_id, StreamIndex index, void* surface);

    /**
     * 
     * @hidden 
     * @brief 写外部日志到RTC日志文件
     */
    BYTERTC_API void writeLog(const char* file_name, int line_num, const char* api_name, const char* level,
                              const char* format, ...);

    /**
     * 
     * @hidden 
     * @brief 上报外部接口调用到trace平台
     */
    BYTERTC_API void reportApiCall(IRTCEngine* ivideo, ExternalReportType report_type, const char* api_name,
                                   const char* msg);

     /**
     * 
     * @hidden 
     * @brief 会议场景开启多分辨率
     */
    BYTERTC_API int enableSimulcastMode(IRTCEngine* ivideo, bool enabled);

    } 
