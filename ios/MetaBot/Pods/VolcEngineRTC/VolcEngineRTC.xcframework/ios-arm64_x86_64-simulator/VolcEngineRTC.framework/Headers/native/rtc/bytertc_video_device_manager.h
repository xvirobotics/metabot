/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Video Device Manager
*/

#pragma once

#include "bytertc_audio_device_manager.h"
#include "bytertc_video_defines.h"

namespace bytertc {
/**
 * 
 * @type api
 * @brief 视频设备相关的信息
 * @list 引擎管理
 */

class IVideoDeviceCollection : public IDeviceCollection {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    IVideoDeviceCollection() {};
    /**
     * 
     * @type api
     * @brief 获取当前系统内视频设备数量
     * @return 视频设备数量
     * @list 引擎管理
     */
    
    virtual int getCount() override {return 0;};
    /**
     * 
     * @type api
     * @brief 根据索引号，获取设备信息
     * @param index 设备索引号，从 0 开始，注意需小于 getCount{@link #IVideoDeviceCollection#getCount} 返回值。
     * @param device_name 设备名称
     * @param device_id 设备 ID
     * @return
     *        - 0：方法调用成功
     *        - !0：方法调用失败
     * @list 引擎管理
     */
    
    virtual int getDevice(int index, char device_name[MAX_DEVICE_ID_LENGTH], char device_id[MAX_DEVICE_ID_LENGTH]) override {return 0;};
    /**
     * 
     * @type api
     * @brief 释放当前 IVideoDeviceCollection{@link #IVideoDeviceCollection} 对象占用的资源。
     * @note 不需要返回视频设备相关信息列表时应该调用本方法释放相关资源。
     * @list 引擎管理
     */
    
    virtual void release()override {};
    /**
     * 
     * @type api
     * @brief 根据索引号，获取设备信息
     * @param index 设备索引号，从 0 开始，注意需小于 getCount{@link #IVideoDeviceCollection#getCount} 返回值。
     * @param video_device_info 设备信息
     * @return
     *        - 0：方法调用成功
     *        - !0：方法调用失败
     * @list 引擎管理
     */
    
    virtual int getDevice(int index, VideoDeviceInfo* video_device_info) = 0;
protected:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IVideoDeviceCollection() {};
};
/**
 * 
 * @type api
 * @brief 主要用于枚举、设置视频采集设备
 * @list 视频管理
 */

class IVideoDeviceManager {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 构造函数
     */
    
    IVideoDeviceManager() {
    }
    /**
     * 
     * @type api
     * @brief 获取当前系统内视频采集设备列表。
     * @return 包含系统中所有视频采集设备的列表，参看 IVideoDeviceCollection{@link #IVideoDeviceCollection}。 <br>
     * 等待超时后会返回空列表。超时时间默认为 10 s。建议通过 onVideoDeviceStateChanged{@link #IRTCEngineEventHandler#onVideoDeviceStateChanged} 监听到 `kMediaDeviceListUpdated` 后，再次调用本接口获取。
     * @note 你可以在收到 onVideoDeviceStateChanged{@link #IRTCEngineEventHandler#onVideoDeviceStateChanged} 了解设备变更后，重新调用本接口以获得新的设备列表。
     * @list 视频管理
     */
    
    virtual IVideoDeviceCollection* enumerateVideoCaptureDevices() = 0;
    /**
     * 
     * @type api
     * @brief 设置当前视频采集设备
     * @param device_id 视频设备 ID，可以通过 enumerateVideoCaptureDevices{@link #IVideoDeviceManager#enumerateVideoCaptureDevices} 获取
     * @return
     *        - 0：方法调用成功
     *        - !0：方法调用失败
     * @list 视频管理
     */
    
    virtual int setVideoCaptureDevice(const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /**
     * 
     * @type api
     * @brief 获取当前 SDK 正在使用的视频采集设备信息
     * @param device_id 视频设备 ID。此参数同时是入参和出参。
     * @return
     *        - 0：方法调用成功
     *        - !0：方法调用失败
     * @list 视频管理
     */
    
    virtual int getVideoCaptureDevice(char device_id[MAX_DEVICE_ID_LENGTH]) = 0;

    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IVideoDeviceManager() {
    }
};


}  