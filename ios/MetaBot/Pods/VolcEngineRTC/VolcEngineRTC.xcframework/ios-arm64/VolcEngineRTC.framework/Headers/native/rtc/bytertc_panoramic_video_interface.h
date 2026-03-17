/*
 *  Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 *  @company ByteDance.Inc
 *  @brief panoramic video interface
 */

#pragma once

namespace bytertc {
/**
 * 
 * @hidden for internal use only on Windows and Android
 * @type keytype
 * @brief 四元数表示的头部位姿信息<br>
 * q = xi + yj + zk + w，i^2 = j^2 = k^2 = −1
 */

typedef struct Quaternionf {    
    /**
     * 
     * @brief x 坐标
     */
    
    float    x;
    /**
     * 
     * @brief y 坐标
     */
    
    float    y;
    /**
     * 
     * @brief z 坐标
     */
    
    float    z;
    /**
     * 
     * @brief 旋转角
     */
    
    float    w;
} Quaternionf;

/**
 * 
 * @hidden (Windows, Android) internal use only
 * @type api
 * @brief 全景视频接口实例
 */

class IPanoramicVideo {
public:
    /** 
     * 
     * @type api
     * @brief 更新接收者的头位姿四元数。RTC 将根据头位姿下发相应的 Tile 视频。
     * @param info 参考 Quaternionf{@link #Quaternionf}。
     */
    
    virtual int updateQuaternionf(const Quaternionf& info) = 0;

    virtual ~IPanoramicVideo() = default;
};
}
