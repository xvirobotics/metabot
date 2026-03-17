/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Internal API
*/

#pragma once

#include "bytertc_defines.h"
#include <stdint.h>
#include <stdarg.h>

namespace bytertc {

/** 
 * @brief 设置 ByteRTC SDK 默认开启使用QUIC获取配置
 * @notes  一般在linux端SDK使用
 */
BYTERTC_API void setDefaultEnableQuicGetAccess();

/** 
 * @brief 设置 ByteRTC SDK 默认开启使用KCP获取配置
 * @notes  一般在linux端SDK使用
 */
BYTERTC_API void setDefaultEnableKcpGetAccess();

/** 
 * @brief 设置 ByteRTC SDK 默认开启通话前网络探测
 * @notes  一般在linux端SDK使用
 */
BYTERTC_API void setDefaultEnableNetworkDetect();

/** 
 * @brief 设置 ByteRTC SDK 默认开启使用KCP消息通道
 * @notes  一般在linux端SDK使用
 */
BYTERTC_API void setDefaultEnableKcpDataChannel();

struct VideoStreamDescription {
    unsigned int width;
    unsigned int height;
    unsigned int frame_rate;
    unsigned int max_kbps;
};

enum class PixelFormat {
      kDefault = 0,
      kI420 = 1,
      kRGBA = 2,
      kTexture = 3,
};

enum class NetworkFamily {
    kNetworkFamilyNone = 0,
    kNetworkFamilyIPv4 = 1,
    kNetworkFamilyIPv6 = 2,
    kNetworkFamilyBoth = 3,
};

enum StreamFlag {
    kStreamFlagNone = 0,
    kStreamFlagNormal = 1 << 0,
    kStreamFlagDocShare = 1 << 1,
    kStreamFlagMCU = 1 << 2,
    kStreamFlagMeetTogether = 1 << 3,
    
    kStreamFlagIntelligentStream = 1 << 6,
};

}  

