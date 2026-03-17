#pragma once

#include <string>

namespace bytertc {

/**
 * 
 * @type keytype
 * @brief 歌曲过滤方式。
 */

enum MusicFilterType {
    /**
     * 
     * @brief 不过滤。
     */
    
    kMusicFilterTypeNone = 0,
    /**
     * 
     * @brief 过滤没有歌词的歌曲。
     */
    
    kMusicFilterTypeWithoutLyric     = 1 << 0,
    /**
     * 
     * @brief 过滤不支持打分的歌曲。
     */
    
    kMusicFilterTypeUnsupportedScore     = 1 << 1,
    /**
     * 
     * @brief 过滤不支持伴唱切换的歌曲。
     */
    
    kMusicFilterTypeUnsupportedAccopmay  = 1 << 2,
    /**
     * 
     * @brief 过滤没有高潮片段的歌曲。
     */
    
    kMusicFilterTypeUnsupportedClimx     = 1 << 3,
};

/**
 * 
 * @type keytype
 * @brief 原唱伴唱类型。
 */

enum class AudioTrackType {
    /**
     * 
     * @brief 播放原唱。
     */
    
    kOriginal = 1,
    /**
     * 
     * @brief 播放伴唱。
     */
    
    kAccompy = 2
};

/**
 * 
 * @type keytype
 * @brief 音乐播放类型。
 */

enum class AudioPlayType {
    /**
     * 
     * @brief 仅本地播放。
     */
    
    kLocal,
    /**
     * 
     * @brief 仅远端播放。
     */
    
    kRemote,
    /**
     * 
     * @brief 本地、远端同时播放。
     */
    
    kLocalAndRemote
};

/**
 * 
 * @type keytype
 * @brief 音乐播放状态。
 */

enum class PlayState {
    /**
     * 
     * @brief 播放中。
     */
    
    kPlaying = 1,
    /**
     * 
     * @brief 暂停中。
     */
    
    kPaused = 2,
    /**
     * 
     * @brief 已停止。
     */
    
    kStoped = 3,
    /**
     * 
     * @brief 播放失败。
     */
    
    kFailed = 4,
    /**
     * 
     * @brief 播放结束。
     */
    
    kFinished = 5,
};

/**
 * 
 * @type keytype
 * @brief 歌词格式类型。
 */

enum class LyricStatus {
    /**
     * 
     * @brief 无歌词。
     */
    
    kNone = 0,
    /**
     * 
     * @brief KRC 歌词。
     */
    
    kKRC = 1,
    /**
     * 
     * @brief LRC 歌词。
     */
    
    kLRC = 2,
    /**
     * 
     * @brief KRC 歌词和 LRC 歌词均有。
     */
    
    kKRCAndLRC = 3
};

/**
 * 
 * @type keytype
 * @brief 歌词文件类型。
 */

enum class DownloadLyricType {
    /**
     * 
     * @brief KRC 歌词文件。
     */
    
    kKRC,
    /**
     * 
     * @brief LRC 歌词文件。
     */
    
    kLRC
};



/**
 * 
 * @type keytype
 * @brief 歌曲数据。
 */

struct MusicInfo
{
    /**
     * 
     * @brief 音乐 ID。
     */
    
    const char*  music_id;
    /**
     * 
     * @brief 音乐名称。
     */
    
    const char*  music_name;
    /**
     * 
     * @brief 歌手。
     */
    
    const char*  singer;
    /**
     * 
     * @brief 版权商 ID。
     */
    
    const char*  vendor_id;
    /**
     * 
     * @brief 版权商名称。
     */
    
    const char* vendor_name;
    /**
     * 
     * @brief 最新更新时间戳，单位为毫秒。
     */
    
    int64_t update_timestamp;
    /**
     * 
     * @brief 封面地址。
     */
    
    const char*  poster_url;
    /**
     * 
     * @brief 歌词格式类型，参看 LyricStatus{@link #LyricStatus}。
     */
    
    LyricStatus lyric_status;
    /**
     * 
     * @brief 歌曲长度，单位为毫秒。
     */
    
    int duration;
    /**
     * 
     * @brief 歌曲是否支持打分。
     */
    
    bool enable_score;
    /**
     * 
     * @brief 歌曲高潮片段开始时间，单位为毫秒。
     */
    
    int climax_start_time;
    /**
     * 
     * @brief 歌曲高潮片段停止时间，单位为毫秒。
     */
    
     int climax_stop_time;

};

/**
 * 
 * @type keytype
 * @brief 热榜歌曲数据。
 */

struct HotMusicInfo {
    /**
     * 
     * @brief 热榜名称。
     */
    
    const char* hot_name;
    /**
     * 
     * @brief 歌曲数据，参看 MusicInfo{@link #MusicInfo}。
     */
    
    MusicInfo* musics;
    /**
     * 
     * @brief 歌曲列表的数量。
     */
    
    int music_count;
};

/**
 * 
 * @type keytype
 * @brief 下载文件类型。
 */

enum DownloadFileType {
    /**
     * 
     * @brief 音频文件。
     */
    
    kDownloadFileTypeMusic = 1,
    /**
     * 
     * @brief KRC 歌词文件。
     */
    
    kDownloadFileTypeKRC = 2,
    /**
     * 
     * @brief LRC 歌词文件。
     */
    
    kDownloadFileTypeLRC = 3,
    /**
     * 
     * @brief MIDI 文件。
     */
    
    kDownloadFileTypeMIDI = 4

} ;

/**
 * 
 * @type keytype
 * @brief 歌曲下载信息。
 */

struct DownloadResult {
    /**
     * 
     * @brief 文件存放路径。
     */
    
    const char* local_file_path;
    /**
     * 
     * @brief 音乐 ID。
     */
    
    const char* music_id;
    /**
     * 
     * @brief 下载文件类型，参看 DownloadFileType{@link #DownloadFileType}。
     */
    
    DownloadFileType type;
};


/**
 * 
 * @type errorcode
 * @brief KTV 错误码。
 */

enum KTVErrorCode {
    /**
     * 
     * @brief 成功。
     */
    
    kKTVErrorCodeOK = 0,
    /**
     * 
     * @brief AppID 异常。
     */
    
    kKTVErrorCodeAppidInValid = -3000,
    /**
     * 
     * @brief 非法参数，传入的参数不正确。
     */
    
    kKTVErrorCodeParasInValid = -3001,
    /**
     * 
     * @brief 获取歌曲资源失败。
     */
    
    kKTVErrorCodeGetMusicFailed = -3002,
    /**
     * 
     * @brief 获取歌词失败。
     */
    
    kKTVErrorCodeGetLyricFailed = -3003,
    /**
     * 
     * @brief 歌曲下架。
     */
    
    kKTVErrorCodeMusicTakedown = -3004,
    /**
     * 
     * @brief 歌曲文件下载失败。
     */
    
    kKTVErrorCodeMusicDownload = -3005,
    /**
     * 
     * @brief MIDI 文件下载失败。
     */
    
    kKTVErrorCodeMidiDownloadFailed = -3006,
    /**
     * 
     * @brief 系统繁忙。
     */
    
    kKTVErrorCodeSystemBusy = -3007,
    /**
     * 
     * @brief 网络异常。
     */
    
    kKTVErrorCodeNetwork = -3008,
    /**
     * 
     * @brief KTV 功能未加入房间。
     */
    
    kKTVErrorCodeNotJoinRoom = -3009,
    /**
     * 
     * @brief 解析数据失败。
     */
    
    kKTVErrorCodeParseData = -3010,
    /**
     * 
     * @hidden
     * @deprecated 从 353 开始。
     * @brief 下载失败。
     */
    
    kKTVErrorCodeDownload = -3011,
    /**
     * 
     * @brief 已在下载中。
     */
    
    kKTVErrorCodeDownloading = -3012,
    /**
     * 
     * @brief 内部错误，联系技术支持人员。
     */
    
    kKTVErrorCodeInternalDomain = -3013,
    /**
     * 
     * @brief 下载失败，磁盘空间不足。清除缓存后重试。
     */
    
    kKTVErrorCodeInsufficientDiskSpace = -3014,
    /**
     * 
     * @brief 下载失败，音乐文件解密失败，联系技术支持人员。
     */
    
    kKTVErrorCodeMusicDecryptionFailed = -3015,
    /**
     * 
     * @brief 下载失败，音乐文件重命名失败，请重试。
     */
    
    kKTVErrorCodeFileRenameFailed = -3016,
    /**
     * 
     * @brief 下载失败，下载超时，请重试。
     */
    
    kKTVErrorCodeDownloadTimeOut = -3017,
    /**
     * 
     * @brief 清除缓存失败，可能原因是文件被占用或者系统异常，请重试。
     */
    
    kKTVErrorCodeClearCacheFailed = -3018,
    /**
     * 
     * @brief 取消下载。
     */
    
    kKTVErrorCodeDownloadCanceled = -3019
};

/**
 * 
 * @type keytype
 * @brief KTV 播放器错误码。
 */

enum KTVPlayerErrorCode {
    /**
     * 
     * @brief 成功。
     */
    
    kKTVPlayerErrorCodeOK = 0,
    /**
     * 
     * @brief 播放错误，请下载后播放。
     */
    
    kKTVPlayerErrorCodeFileNotExist = -3020,
    /**
     * 
     * @brief 播放错误，请确认文件播放格式。
     */
    
    kKTVPlayerErrorCodeFileError = -3021,
    /**
     * 
     * @brief 播放错误，未进入房间。
     */
    
    kKTVPlayerErrorCodeNotJoinRoom = -3022,
    /**
     * 
     * @brief 参数错误。
     */
    
    kKTVPlayerErrorCodeParam = -3023,
    /**
     * 
     * @brief 播放失败，找不到文件或文件打开失败。
     */
    
    kKTVPlayerErrorCodeStartError = -3024,
    /**
     * 
     * @brief 混音 ID 异常。
     */
    
    kKTVPlayerErrorCodeMixIdError = -3025,
    /**
     * 
     * @brief 设置播放位置出错。
     */
    
    kKTVPlayerErrorCodePositionError = -3026,
    /**
     * 
     * @brief 音量参数不合法，可设置的取值范围为 [0,400]。
     */
    
    kKTVPlayerErrorCodeAudioVolumeError = -3027,
    /**
     * 
     * @brief 不支持此混音类型。
     */
    
    kKTVPlayerErrorCodeTypeError = -3028,
    /**
     * 
     * @brief 音调文件不合法。
     */
    
    kKTVPlayerErrorCodePitchError = -3029,
    /**
     * 
     * @brief 音轨不合法。
     */
    
    kKTVPlayerErrorCodeAudioTrackError = -3030,
    /**
     * 
     * @brief 混音启动中。
     */
    
    kKTVPlayerErrorCodeStartingError = -3031
};


} 