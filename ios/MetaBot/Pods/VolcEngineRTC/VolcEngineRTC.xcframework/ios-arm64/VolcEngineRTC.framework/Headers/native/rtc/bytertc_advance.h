/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Advance API
*/

#pragma once

#include "../bytertc_engine.h"

#include <stdint.h>
#ifdef __ANDROID__
#include "jni.h"
#endif


#ifdef __GNUC__
#    define attribute_deprecated __attribute__((deprecated))
#elif defined(_MSC_VER)
#    define attribute_deprecated __declspec(deprecated)
#else
#    define attribute_deprecated
#endif

namespace bytertc {
/**
 * 
 * @hidden for internal use only
 * @brief 设置应用的状态
 * @param engine <br>
 *       要通知的引擎
 * @param app_state 应用状态字符串
 */

BYTERTC_API void setAppState(void* engine, const char* app_state);
/**
 * 
 * @hidden(iOS,macOS,Windows,Linux) for internal use only
 * @type api
 * @brief 用于设置 Android 的 ApplicationContext 给 Engine 使用， <br>
 * 如果 Android 使用纯 C++接口，则该方法必须在引擎创建前设置
 * @param j_egl_context <br>
 *      传入 Java 的 android.content.Context 类型的 EGLContext
 * @list 引擎管理
 */

BYTERTC_API void setApplicationContext(void* j_application_context);
BYTERTC_API int setLogConfig(const LogConfig& log_config);

/**
 * 
 * @region 视频管理
 * @type keytype
 * @brief 硬件加速设备类型
 */

enum HWDeviceType {
    /**
     * 
     * @brief 未知的设备类型
     */
    
    kHWDeviceTypeNone,

    /**
     * 
     * @brief direct3d 11 视频加速设备
     */
    
    kHWDeviceTypeD3D11,

    /**
     * 
     * @brief cuda 硬件加速设备
     * @note cuda 是 nvidia 平台下硬件加速设备
     */
    
    kHWDeviceTypeCuda,

    /**
     * 
     * @brief intel 平台下 qsv 加速设备
     */
    
    kHWDeviceTypeQsv,

    /**
     * 
     * @brief windows 平台 dx 硬件加速设备
     */
    
    kHWDeviceTypeDxva2,

    /**
     * 
     * @brief Android 平台下硬件加速设备 mediacodec
     */
    
    kHWDeviceTypeMediaCodec,

    /**
     * 
     * @brief IOS、MACOS 平台下硬件加速设备 VideoToolbox
     */
    
    kHWDeviceTypeVideoToolbox,
     /**
     * 
     * @brief android、linux 平台下硬件加速设备 egl context
     */
    
    kHWDeviceTypeEglContext,

    /**
     * 
     * @brief linux、windows 平台下硬件加速设备 va context
     */
    
    kHWDeviceTypeVaapi,
};

/**
 * 
 * @hidden(macOS, Linux, iOS, Android)
 * @type keytype
 * @brief 硬编码设备 context
 */

enum HWDeviceContextKey {
    /**
     * 
     * @brief cuda context
     */
         
    kHWDeviceContextKeyCudaContext,
    /**
     * 
     * @brief derict3D11 设备
     */
    
    kHWDeviceContextKeyD3D11Device,
    /**
     * 
     * @brief derict3D11 设备 context
     */
    
    kHWDeviceContextKeyD3D11DeviceContext,
    /**
     * 
     * @brief egl 设备 context
     */
    
    kHWDeviceContextKeyEglContext,
    /**
     * 
     * @brief vadisplay 设备 context
     */
    
    kHWDeviceContextKeyVADisplay,
};

/**
 * 
 * @hidden(macOS, Linux, iOS, Android)
 * @type keytype
 * @brief 硬件加速设备 context
 */

class IHWDeviceContext {
public:
    virtual ~IHWDeviceContext() = default;
    /**
     * 
     * @brief 获取硬件加速设备 context 类型
     * @return 硬件加速设备类型，详见 HWDeviceType{@link #HWDeviceType}
     */
    
    virtual HWDeviceType deviceType() const = 0;
    /**
     * 
     * @brief 设置属性
     * @param key 硬件加速设备 context 类型，详见 HWDeviceContextKey{@link #HWDeviceContextKey}
     * @param value 指向硬件加速设备 context 地址的指针
     */
    
    virtual void setProperty(HWDeviceContextKey key, void* value) = 0;
    /**
     * 
     * @brief 获取属性
     * @param key 硬件加速设备 context 类型，详见 HWDeviceContextKey{@link #HWDeviceContextKey}
     * @return 返回硬件加速设备 context 地址的指针
     */
    
    virtual void* getProperty(HWDeviceContextKey key) = 0;
    /**
     * 
     * @brief 内部 context 是否由该实例拥有
     * @return
     *        - true: 硬件设备 context 被实例对象拥有
     *        - false: 硬件设备上 context 文不被实例对象拥有
     */
    
    virtual bool ownContext() const = 0;
    /**
     * 
     * @brief 设置 context 是否被实例对象拥有
     * @param own_context <br>
     *       - true: context 被实例对象拥有
     *       - false: context 不被实例对象拥有
     */
    
    virtual void setOwnContext(bool own_context) = 0;
    /**
     * 
     * @brief 转移硬件设备 context，
     * @return 指向硬件设备 context 的指针
     */
    
    virtual IHWDeviceContext* moveContext() = 0;
    /**
     * 
     * @brief 释放实例对象
     */
    
    virtual void release() = 0;
};

/**
 * 
 * @hidden for internal use only
 * @brief 创建一个具有指定设备类型的硬件设备上下文
 */
 

BYTERTC_API IHWDeviceContext* createHwDeviceContext(HWDeviceType device_type);

/**
 * 
 * @hidden for internal use only
 * @brief 设置视频源的设备上下文信息
 * @param engine <br>
 *       要设置的引擎，详见 IRTCEngine{@link #IRTCEngine}
 * @param hw_context <br>
 *        设置视频帧，详见：IHWDeviceContext{@link #IHWDeviceContext}
 * @note 用于硬件编码传入 GPU 内存时使用
 */

BYTERTC_API void setVideoSourceHWDeviceContext(void* engine, IHWDeviceContext* hw_context);

/**
 * 
 * @hidden for internal use only
 * @type callback
 * @brief 本地音频帧监测器
 * @list 自定义流处理
 */

class ILocalEncodedAudioFrameObserver  {
public:
    virtual ~ILocalEncodedAudioFrameObserver() {
    }
    /**
     * 
     * @type callback
     * @brief 调用 RegisterLocalEncodedAudioFrameObserver 后，SDK 收到本地音频帧信息时，回调该事件
     * @param audio_source 本地音频源类型，详见 IAudioSource{@link #IAudioSource}
     * @param audio_stream 本地音频帧信息，参看 IEncodedAudioFrame
     * @list 自定义流处理
     */
    
    virtual void onLocalEncodedAudioFrame(IAudioSource* audio_source, const EncodedAudioFrameData& audio_stream) = 0;
};
}  

