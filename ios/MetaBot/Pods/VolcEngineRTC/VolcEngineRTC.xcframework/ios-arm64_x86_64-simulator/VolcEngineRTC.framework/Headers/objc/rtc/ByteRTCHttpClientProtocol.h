/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC HTTP Client Protocol
*/

#import <Foundation/Foundation.h>
/**
 * 
 * @hidden for internal use only
 * @type api
 * @list 引擎管理
 */

@protocol ByteRTCHttpClientProtocol <NSObject>

@required
/**
 * 
 * @type api
 * @brief 需要实现的 HTTP 异步 GET 接口。
 * @param url GET 请求地址。
 * @param timeout 超时时间。
 * @param callback GET 请求结果回调函数。
 * @list 引擎管理
 */

- (void)getAsync:(NSString * _Nonnull)url timeoutMillisecond:(int)timeout
    withCallback:(void(^ _Nullable)(int code, NSString* _Nullable data))callback;
/**
 * 
 * @type api
 * @brief 需要实现的 HTTP 异步 POST 接口。
 * @param url  POST 请求地址。
 * @param content POST 请求的内容。
 * @param timeout 超时时间。
 * @param callback  POST 请求结果回调函数。
 * @list 引擎管理
 */

- (void)postAsync:(NSString * _Nonnull)url content:(NSString * _Nullable)content timeoutMillisecond:(int)timeout
    withCallback:(void(^ _Nullable)(int code, NSString* _Nullable data))callback;
@end