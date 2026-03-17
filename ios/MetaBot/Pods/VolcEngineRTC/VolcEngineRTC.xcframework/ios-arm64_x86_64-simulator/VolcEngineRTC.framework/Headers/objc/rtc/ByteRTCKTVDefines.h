/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief ByteRTCRtsDefines
*/

#import "ByteRTCRtsDefines.h"

/**
 * 
 * @type keytype
 * @brief 歌曲过滤方式。
 */

typedef NS_OPTIONS(NSUInteger, ByteRTCMusicFilterType) {
    /**
     * 
     * @brief 不过滤。
     */
    
    ByteRTCMusicFilterTypeNone                 = 0,
    /**
     * 
     * @brief 过滤没有歌词的歌曲。
     */
    
    ByteRTCMusicFilterTypeWithoutLyric         = 1 << 0,
    /**
     * 
     * @brief 过滤不支持打分的歌曲。
     */
    
    ByteRTCMusicFilterTypeUnsupportedScore     = 1 << 1,
    /**
     * 
     * @brief 过滤不支持伴唱切换的歌曲。
     */
    
    ByteRTCMusicFilterTypeUnsupportedAccopmay  = 1 << 2,
    /**
     * 
     * @brief 过滤没有高潮片段的歌曲。
     */
    
    ByteRTCMusicFilterTypeUnsupportedClimx     = 1 << 3,
};

/**
 * 
 * @type keytype
 * @brief 音乐播放状态。
 */

typedef NS_ENUM(NSInteger, ByteRTCPlayState) {
    /**
     * 
     * @brief 播放中。
     */
    
    ByteRTCPlayStatePlaying    = 1,
    /**
     * 
     * @brief 暂停中。
     */
    
    ByteRTCPlayStatePaused     = 2,
    /**
     * 
     * @brief 已停止。
     */
    
    ByteRTCPlayStateStoped     = 3,
    /**
     * 
     * @brief 播放失败。
     */
    
    ByteRTCPlayStateFailed     = 4,
    /**
     * 
     * @brief 播放结束。
     */
    
    ByteRTCPlayStateFinished   = 5,
};

/**
 * 
 * @type keytype
 * @brief 原唱伴唱类型。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioTrackType) {
    /**
     * 
     * @brief 播放原唱。
     */
    
    ByteRTCAudioTrackTypeOriginal    = 1,
    /**
     * 
     * @brief 播放伴唱。
     */
    
    ByteRTCAudioTrackTypeAccompy     = 2,
};

/**
 * 
 * @type keytype
 * @brief 音乐播放类型。
 */

typedef NS_ENUM(NSInteger, ByteRTCAudioPlayType) {
    /**
     * 
     * @brief 仅本地播放。
     */
    
    ByteRTCAudioPlayTypeLocal            = 0,
    /**
     * 
     * @brief 仅远端播放。
     */
    
    ByteRTCAudioPlayTypeRemote           = 1,
    /**
     * 
     * @brief 本地、远端同时播放。
     */
    
    ByteRTCAudioPlayTypeLocalAndRemote   = 2,
};

/**
 * 
 * @type keytype
 * @brief 歌词格式类型。
 */

typedef NS_ENUM(NSInteger, ByteRTCLyricStatus) {
    /**
     * 
     * @brief 无歌词。
     */
    
    ByteRTCLyricStatusNone         = 0,
    /**
     * 
     * @brief KRC 歌词。
     */
    
    ByteRTCLyricStatusKRC          = 1,
    /**
     * 
     * @brief LRC 歌词。
     */
    
    ByteRTCLyricStatusLRC          = 2,
    /**
     * 
     * @brief KRC 歌词和 LRC 歌词均有。
     */
    
    ByteRTCLyricStatusKRCAndLRC    = 3,
};

/**
 * 
 * @type keytype
 * @brief 下载文件类型。
 */

typedef NS_ENUM(NSInteger, ByteRTCDownloadFileType) {
    /**
     * 
     * @brief 音频文件。
     */
    
    ByteRTCDownloadFileTypeMusic     = 1,
    /**
     * 
     * @brief KRC 歌词文件。
     */
    
    ByteRTCDownloadFileTypeKRC       = 2,
    /**
     * 
     * @brief LRC 歌词文件。
     */
    
    ByteRTCDownloadFileTypeLRC       = 3,
    /**
     * 
     * @brief MIDI 文件。
     */
    
    ByteRTCDownloadFileTypeMIDI      = 4,
};

/**
 * 
 * @type keytype
 * @brief 歌词文件类型。
 */

typedef NS_ENUM(NSInteger, ByteRTCDownloadLyricType) {
    /**
     * 
     * @brief KRC 歌词文件。
     */
    
    ByteRTCDownloadLyricTypeKRC = 0,
    /**
     * 
     * @brief LRC 歌词文件。
     */
    
    ByteRTCDownloadLyricTypeLRC = 1,
};

/**
 * 
 * @type errorcode
 * @brief KTV 错误码。
 */

typedef NS_ENUM(NSInteger, ByteRTCKTVErrorCode) {
    /**
     * 
     * @brief 成功。
     */
    
    ByteRTCKTVErrorCodeOK = 0,
    /**
     * 
     * @brief AppID 异常。
     */
    
    ByteRTCKTVErrorCodeAppidInValid = -3000,
    /**
     * 
     * @brief 非法参数，传入的参数不正确。
     */
    
    ByteRTCKTVErrorCodeParasInValid = -3001,
    /**
     * 
     * @brief 获取歌曲资源失败。
     */
    
    ByteRTCKTVErrorCodeGetMusicFailed = -3002,
    /**
     * 
     * @brief 获取歌词失败。
     */
    
    ByteRTCKTVErrorCodeGetLyricFailed = -3003,
    /**
     * 
     * @brief 歌曲下架。
     */
    
    ByteRTCKTVErrorCodeMusicTakedown = -3004,
    /**
     * 
     * @brief 歌曲文件下载失败。
     */
    
    ByteRTCKTVErrorCodeMusicDownload = -3005,
    /**
     * 
     * @brief MIDI 文件下载失败。
     */
    
    ByteRTCKTVErrorCodeMidiDownloadFailed = -3006,
    /**
     * 
     * @brief 系统繁忙。
     */
    
    ByteRTCKTVErrorCodeSystemBusy = -3007,
    /**
     * 
     * @brief 网络异常。
     */
    
    ByteRTCKTVErrorCodeNetwork = -3008,
    /**
     * 
     * @brief KTV 功能未加入房间。
     */
    
    ByteRTCKTVErrorCodeNotJoinRoom = -3009,
    /**
     * 
     * @brief 解析数据失败。
     */
    
    ByteRTCKTVErrorCodeParseData = -3010,
    /**
     * 
     * @hidden
     * @deprecated 从 353 开始
     * @brief 下载失败。
     */
    
    ByteRTCKTVErrorCodeDownload = -3011,
    /**
     * 
     * @brief 已在下载中。
     */
    
    ByteRTCKTVErrorCodeDownloading = -3012,
    /**
     * 
     * @brief 内部错误，联系技术支持人员。
     */
    
    ByteRTCKTVErrorCodeInternal = -3013,
    /**
     * 
     * @brief 下载失败，磁盘空间不足。清除缓存后重试。
     */
    
    ByteRTCKTVErrorCodeInsufficientDiskSpace = -3014,
    /**
     * 
     * @brief 下载失败，音乐文件解密失败，联系技术支持人员。
     */
    
    ByteRTCKTVErrorCodeMusicDecryptionFailed = -3015,
    /**
     * 
     * @brief 下载失败，音乐文件重命名失败，请重试。
     */
    
    ByteRTCKTVErrorCodeFileRenameFailed = -3016,
    /**
     * 
     * @brief 下载失败，下载超时，请重试。
     */
    
    ByteRTCKTVErrorCodeDownloadTimeOut = -3017,
    /**
     * 
     * @brief 清除缓存失败，可能原因是文件被占用或者系统异常，请重试。
     */
    
    ByteRTCKTVErrorCodeClearCacheFailed = -3018,
    /**
     * 
     * @brief 取消下载。
     */
    
    ByteRTCKTVErrorCodeDownloadCanceled = -3019
};

/**
 * 
 * @type errorcode
 * @brief KTV 播放器错误码。
 */

typedef NS_ENUM(NSInteger, ByteRTCKTVPlayerErrorCode) {
    /**
     * 
     * @brief 成功。
     */
    
    ByteRTCKTVPlayerErrorCodeOK = 0,
    /**
     * 
     * @brief 播放错误，请下载后播放。
     */
    
    ByteRTCKTVPlayerErrorCodeFileNotExist = -3020,
    /**
     * 
     * @brief 播放错误，请确认文件播放格式。
     */
    
    ByteRTCKTVPlayerErrorCodeFileError = -3021,
    /**
     * 
     * @brief 播放错误，未进入房间。
     */
    
    ByteRTCKTVPlayerErrorCodeNotJoinRoom = -3022,
    /**
     * 
     * @brief 参数错误。
     */
    
    ByteRTCKTVPlayerErrorCodeParam = -3023,
    /**
     * 
     * @brief 播放失败，找不到文件或文件打开失败。
     */
    
    ByteRTCKTVPlayerErrorCodeStartError = -3024,
    /**
     * 
     * @brief 混音 ID 异常。
     */
    
    ByteRTCKTVPlayerErrorCodeMixIdError = -3025,
    /**
     * 
     * @brief 设置播放位置出错。
     */
    
    ByteRTCKTVPlayerErrorCodePositionError = -3026,
    /**
     * 
     * @brief 音量参数不合法，可设置的取值范围为 [0,400]。
     */
    
    ByteRTCKTVPlayerErrorCodeAudioVolumeError = -3027,
    /**
     * 
     * @brief 不支持此混音类型。
     */
    
    ByteRTCKTVPlayerErrorCodeTypeError = -3028,
    /**
     * 
     * @brief 音调文件不合法。
     */
    
    ByteRTCKTVPlayerErrorCodePitchError = -3029,
    /**
     * 
     * @brief 音轨不合法。
     */
    
    ByteRTCKTVPlayerErrorCodeAudioTrackError = -3030,
    /**
     * 
     * @brief 混音启动中。
     */
    
    ByteRTCKTVPlayerErrorCodeStartingError = -3031,
};

/**
 * 
 * @type keytype
 * @brief 歌曲数据。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCMusicInfo : NSObject
/**
 * 
 * @brief 音乐 ID。
 */

@property (nonatomic, copy) NSString * _Nonnull musicId;
/**
 * 
 * @brief 音乐名称。
 */

@property (nonatomic, copy) NSString * _Nonnull musicName;
/**
 * 
 * @brief 歌手。
 */

@property (nonatomic, copy) NSString * _Nonnull singer;
/**
 * 
 * @brief 版权商 ID。
 */

@property (nonatomic, copy) NSString * _Nonnull vendorId;
/**
 * 
 * @brief 版权商名称。
 */

@property (nonatomic, copy) NSString * _Nonnull vendorName;
/**
 * 
 * @brief 最新更新时间戳，单位为毫秒。
 */

@property (nonatomic, assign) int64_t updateTimestamp;
/**
 * 
 * @brief 封面地址。
 */

@property (nonatomic, copy) NSString * _Nonnull posterUrl;
/**
 * 
 * @brief 歌词格式类型，参看 ByteRTCLyricStatus{@link #ByteRTCLyricStatus}。
 */

@property (nonatomic, assign) ByteRTCLyricStatus lyricStatus;
/**
 * 
 * @brief 歌曲长度，单位为毫秒。
 */

@property (nonatomic, assign) int duration;
/**
 * 
 * @brief 歌曲是否支持打分。
 */

@property (nonatomic, assign) BOOL enableScore;
/**
 * 
 * @brief 歌曲高潮片段开始时间，单位为毫秒。
 */

@property (nonatomic, assign) int climaxStartTime;
/**
 * 
 * @brief 歌曲高潮片段停止时间，单位为毫秒。
 */

@property (nonatomic, assign) int climaxEndTime;
@end

/**
 * 
 * @type keytype
 * @brief 热榜歌曲数据。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCHotMusicInfo : NSObject
/**
 * 
 * @brief 热榜名称。
 */

@property (nonatomic, copy) NSString * _Nullable hotName;
/**
 * 
 * @brief 歌曲数据，参看 ByteRTCMusicInfo{@link #ByteRTCMusicInfo}。
 */

@property (nonatomic, copy) NSArray<ByteRTCMusicInfo *> * _Nullable musics;
@end

/**
 * 
 * @type keytype
 * @brief 歌曲下载信息。
 */

BYTERTC_APPLE_EXPORT @interface ByteRTCDownloadResult : NSObject
/**
 * 
 * @brief 音乐 ID。
 */

@property (nonatomic, copy) NSString * _Nonnull musicId;
/**
 * 
 * @brief 下载文件类型，参看 ByteRTCDownloadFileType{@link #ByteRTCDownloadFileType}。
 */

@property (nonatomic, assign) ByteRTCDownloadFileType fileType;
/**
 * 
 * @brief 文件存放路径。
 */

@property (nonatomic, copy) NSString * _Nullable filePath;

@end