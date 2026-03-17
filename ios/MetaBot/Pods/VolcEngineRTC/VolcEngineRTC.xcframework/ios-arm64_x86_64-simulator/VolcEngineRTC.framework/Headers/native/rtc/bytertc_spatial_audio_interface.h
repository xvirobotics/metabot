/*
 *  Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 *  @company ByteDance.Inc
 *  @brief spatial audio interface
 */

#pragma once

#include "bytertc_audio_defines.h"

namespace bytertc {
/**
 * 
 * @type keytype
 * @brief 用户在空间音频坐标系里的位置信息。
 */

struct PositionInfo {
    /**
     * 
     * @brief 用户在空间音频坐标系里的位置，需自行建立空间直角坐标系。参看 Position{@link #Position}
     */
    
    Position position;
    /**
     * 
     * @brief 用户在空间音频坐标系里的三维朝向信息。三个向量需要两两垂直。参看 HumanOrientation{@link #HumanOrientation}。
     */
    
    HumanOrientation orientation;
};
/**
 * 
 * @hidden(Linux)
 * @type api
 * @brief 空间音频接口实例
 * @list 
 */

class ISpatialAudio {
public:
    /**
     * 
     * @type api
     * @brief 开启/关闭空间音频功能。
     * @param enable 是否开启空间音频功能： <br>
     *        - true：开启
     *        - false：关闭（默认）
     * @note 该方法仅开启空间音频功能，你须调用 updateSelfPosition{@link #ISpatialAudio#updateSelfPosition} 设置自身位置坐标后方可收听空间音频效果。空间音频相关 API 和调用时序详见[空间音频](https.www.volcengine.com/docs/6348/93903)。
     * @list 高级功能
     */
    
    virtual void enableSpatialAudio(bool enable) = 0;
    /**
     * 
     * @type api
     * @brief 关闭本地用户朝向对本地用户发声效果的影响。 <br>
     *        调用此接口后，房间内的其他用户收听本地发声时，声源都在收听者正面。
     * @note
     *        - 调用本接口关闭朝向功能后，在当前的空间音频实例的生命周期内无法再次开启。
     *        - 调用此接口不影响本地用户收听朝向的音频效果。要改变本地用户收听朝向，参看 updateSelfPosition{@link #ISpatialAudio#updateSelfPosition} 和 updateRemotePosition{@link #ISpatialAudio#updateRemotePosition} 。
     * @list 高级功能
     */
    
    virtual void disableRemoteOrientation() = 0;
    /**
     * 
     * @hidden(Linux)
     * @valid since 3.52
     * @type api
     * @brief 设置本地用户在自建空间直角坐标系中的收听坐标和收听朝向，以实现本地用户预期的空间音频收听效果。
     * @param position_info 空间音频位置信息。参看 PositionInfo{@link #PositionInfo}。
     * @return
     *        - 0：成功。
     *        - <0：失败。
     *        - -2: 失败，原因是校验本地用户的三维朝向信息时，三个向量没有两两垂直。
     * @note
     *        - 该方法需在进房后调用。
     *        - 调用该接口更新坐标前，你需调用 enableSpatialAudio{@link #ISpatialAudio#enableSpatialAudio} 开启空间音频功能。空间音频相关 API 和调用时序详见[空间音频](https:
     *        - 调用此接口在本地进行的设定对其他用户的空间音频收听效果不会产生任何影响。
     * @list 高级功能
     */
    
    virtual int updateSelfPosition(const PositionInfo& position_info) = 0;
    /**
     * 
     * @hidden(Linux)
     * @valid since 3.52
     * @type api
     * @brief 设置房间内某一远端用户在本地用户自建的空间音频坐标系中的发声位置和发声朝向，以实现本地用户预期的空间音频收听效果。
     * @param uid 用户 ID
     * @param position_info 远端用户的空间音频位置信息。参看 PositionInfo{@link #PositionInfo}。
     * @return
     *        - 0：成功。
     *        - <0：失败。
     *        - -2: 失败，原因是校验远端用户的三维朝向信息时，三个向量没有两两垂直。
     * @note
     *        - 该方法需在创建房间后调用。
     *        - 调用此接口在本地进行的设定对其他用户的空间音频收听效果不会产生任何影响。
     * @list 高级功能
     */
    
    virtual int updateRemotePosition(const char* uid, const PositionInfo& position_info) = 0;
    /**
     * 
     * @hidden(Linux)
     * @valid since 3.52
     * @type api
     * @brief 移除调用 updateRemotePosition{@link #ISpatialAudio#updateRemotePosition} 为某一远端用户设置的空间音频效果。
     * @param uid 远端用户 ID。
     * @return
     *        - 0：成功。
     *        - <0：失败。
     * @list 高级功能
     */
    
    virtual int removeRemotePosition(const char* uid) = 0;
    /**
     * 
     * @hidden(Linux)
     * @valid since 3.52
     * @type api
     * @brief 移除调用 updateRemotePosition{@link #ISpatialAudio#updateRemotePosition} 为所有远端用户设置的空间音频效果。
     * @return
     *        - 0：成功。
     *        - <0：失败。
     * @list 高级功能
     */
    
    virtual int removeAllRemotePosition() = 0;

    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~ISpatialAudio() = default;
};

}  