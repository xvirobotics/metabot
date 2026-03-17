#pragma once

#include "bytertc_ktv_defines.h"
namespace bytertc {
/**
 * 
 * @type callback
 * @brief KTV 事件回调接口。 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list 在线 KTV
 */

class IKTVManagerEventHandler {

public:
    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~IKTVManagerEventHandler() {
    }
    /**
     * 
     * @type callback
     * @brief 歌曲列表回调。
     * @param music_infos 歌曲数据数组，参看 MusicInfo{@link #MusicInfo}。
     * @param music_count 当前查询的数据条目数。
     * @param total_musics_size 数据条目总数。
     * @param error_code 错误码，成功时返回 0，其余值参看 KTVErrorCode{@link #KTVErrorCode}。
     * @list 在线 KTV
     */
    
    virtual void onMusicListResult(MusicInfo* music_infos, int music_count, int total_musics_size, KTVErrorCode error_code) {
        (void)music_infos;
        (void)music_count;
        (void)total_musics_size;
        (void)error_code;
    }
    /**
     * 
     * @type callback
     * @brief 搜索歌曲结果回调。
     * @param music_infos 歌曲数据数组，参看 MusicInfo{@link #MusicInfo}。
     * @param music_count 当前查询的数据条目数。
     * @param total_musics_size 数据条目总数。
     * @param error_code 错误码，成功时返回 0，其余值参看 KTVErrorCode{@link #KTVErrorCode}。
     * @list 在线 KTV
     */
    
    virtual void onSearchMusicResult(MusicInfo* music_infos, int music_count, int total_musics_size, KTVErrorCode error_code) {
        (void)music_infos;
        (void)music_count;
        (void)total_musics_size;
        (void)error_code;
    }
    /**
     * 
     * @type callback
     * @brief 热榜歌曲结果回调。
     * @param hot_infos 热榜歌曲数据数组，参看 HotMusicInfo{@link #HotMusicInfo}。
     * @param hot_count 当前查询的热榜歌曲数据条目数。
     * @param error_code 错误码，成功时返回 0，其余值参看 KTVErrorCode{@link #KTVErrorCode}。
     * @list 在线 KTV
     */
    
    virtual void onHotMusicResult(HotMusicInfo* hot_infos, int hot_count, KTVErrorCode error_code) {
        (void)hot_infos;
        (void)hot_count;
        (void)error_code;
    }
    /**
     * 
     * @type callback
     * @brief 歌曲详细信息回调。
     * @param music_info 歌曲数据，参看 MusicInfo{@link #MusicInfo}。
     * @param error_code 错误码，成功时返回 0，其余值参看 KTVErrorCode{@link #KTVErrorCode}。 <br>
     * @list 在线 KTV
     */
    
    virtual void onMusicDetailResult(MusicInfo* music_info, KTVErrorCode error_code) {
        (void)music_info;
        (void)error_code;        
    }
    /**
     * 
     * @type callback
     * @brief 下载成功回调。
     * @param download_id 下载任务 ID。
     * @param download_info 下载信息，参看 DownloadResult{@link #DownloadResult}。
     * @list 在线 KTV
     */
    
    virtual void onDownloadSuccess(int download_id, DownloadResult* download_info) {
        (void)download_id;
        (void)download_info; 
    }
    /**
     * 
     * @type callback
     * @brief 下载失败回调。
     * @param download_id 下载任务 ID。
     * @param error_code 错误码，参看 KTVErrorCode{@link #KTVErrorCode}。
     * @list 在线 KTV
     */
    
    virtual void onDownloadFailed(int download_id, KTVErrorCode error_code) {
        (void)download_id;
        (void)error_code; 
    }
    /**
     * 
     * @type callback
     * @brief 歌曲文件下载进度回调。
     * @param download_id 下载任务 ID。
     * @param download_percentage 下载进度百分比，取值范围 [0,100]。
     * @list 在线 KTV
     */
    
    virtual void onDownloadMusicProgress(int download_id, int download_percentage) {
        (void)download_id;
        (void)download_percentage;
    }
    /**
     * 
     * @type callback
     * @brief 清理文件缓存结果回调。
     * @param error_code 错误码，非 0 为失败，参看 KTVErrorCode{@link #KTVErrorCode}。
     * @list Online Karaoke
     */
    
    virtual void onClearCacheResult(KTVErrorCode error_code) {
        (void)error_code;
    }
};
/**
 * 
 * @type callback
 * @brief KTV 播放器回调接口。 <br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 * @list Online Karaoke
 */

class IKTVPlayerEventHandler {
public:
    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~IKTVPlayerEventHandler() {
    }
    /**
     * 
     * @type callback
     * @brief 音乐播放进度回调。
     * @param music_id 音乐 ID。
     * @param progress 音乐播放进度，单位为毫秒。
     * @list 在线 KTV
     */
    
    virtual void onPlayProgress(const char *music_id, int64_t progress) {
        (void)music_id;
        (void)progress;
    }
    /**
     * 
     * @type callback
     * @brief 音乐播放状态改变回调。
     * @param music_id 音乐 ID。
     * @param play_state 音乐播放状态，参看 PlayState{@link #PlayState}。
     * @param error_code 错误码，参看 KTVPlayerErrorCode{@link #KTVPlayerErrorCode}。
     * @note
     *       此回调被触发的时机汇总如下： <br>
     *       - 调用 playMusic{@link #IKTVPlayer#playMusic} 成功后，会触发 playState 值为 PlayStatePlaying 的回调；否则会触发 playState 值为 PlayStateFailed 的回调。
     *       - 使用相同的音乐 ID 重复调用 playMusic{@link #IKTVPlayer#playMusic} 后，后一次播放会覆盖前一次，且会触发 playState 值为 PlayStatePlaying 的回调，表示后一次音乐播放已开始。
     *       - 调用 pauseMusic{@link #IKTVPlayer#pauseMusic} 方法暂停播放成功后，会触发 playState 值为 PlayStatePaused 的回调；否则触发 playState 值为 PlayStateFailed 的回调。
     *       - 调用 resumeMusic{@link #IKTVPlayer#resumeMusic} 方法恢复播放成功后，会触发 playState 值为 PlayStatePlaying 的回调；否则触发 playState 值为 PlayStateFailed 的回调。
     *       - 调用 stopMusic{@link #IKTVPlayer#stopMusic} 方法停止播放成功后，会触发 playState 值为 PlayStateStoped 的回调；否则触发 playState 值为 PlayStateFailed 的回调。
     *       - 音乐播放结束会触发 playState 值为 PlayStateFinished 的回调。
     * @list 在线 KTV
     */
    
    virtual void onPlayStateChanged(const char* music_id, PlayState play_state, bytertc::KTVPlayerErrorCode error_code) {
        (void)music_id;
        (void)play_state;
        (void)error_code;
    }
};
/**
 * 
 * @type api
 * @brief KTV 播放器接口。
 * @list Online Karaoke
 */

class IKTVPlayer {
public:
    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~IKTVPlayer() = default;
    /**
     * 
     * @type api
     * @brief 设置 KTV 播放器进度及状态回调接口。
     * @param handler KTV 播放器回调类，参看 IKTVPlayerEventHandler{@link #IKTVPlayerEventHandler}。
     * @list 在线 KTV
     */
    
    virtual void setPlayerEventHandler(IKTVPlayerEventHandler *handler) = 0;
    /**
     * 
     * @type api
     * @brief 播放歌曲。
     * @param music_id 音乐 ID。 <br>
     *        若同一 music_id 的歌曲正在播放，再次调用接口会从开始位置重新播放。若 music_id 对应的音频文件不存在会触发报错。
     * @param track_type 原唱伴唱类型，参看 AudioTrackType{@link #AudioTrackType}。
     * @param play_type 音乐播放类型。参看 AudioPlayType{@link #AudioPlayType}。
     * @note
     *        - 调用接口后，你会收到 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调歌曲播放状态。
     *        - 若音乐 ID 错误，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3023，play_state 为 4。
     *        - 若未进房，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3022，play_state 为 4。
     *        - 若音乐文件不存在，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3020，play_state 为 4。
     * @list 在线 KTV
     */
    
    virtual void playMusic(const char* music_id, AudioTrackType track_type, AudioPlayType play_type) = 0;
    /**
     * 
     * @type api
     * @brief 暂停播放歌曲。
     * @param music_id 音乐 ID。
     * @note
     *        - 调用接口后，你会收到 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调歌曲播放状态。
     *        - 若音乐 ID 错误，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3023，play_state 为 4。
     *        - 若未进房，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3022，play_state 为 4。
     * @list 在线 KTV
     */
    
    virtual void pauseMusic(const char *music_id) = 0;
    /**
     * 
     * @type api
     * @brief 继续播放歌曲。
     * @param music_id 音乐 ID。
     * @note
     *        - 调用接口后，你会收到 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调歌曲播放状态。
     *        - 若音乐 ID 错误，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3023，play_state 为 4。
     *        - 若未进房，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3022，play_state 为 4。
     * @list 在线 KTV
     */
    
    virtual void resumeMusic(const char *music_id) = 0;
    /**
     * 
     * @type api
     * @brief 停止播放歌曲。
     * @param music_id 音乐 ID。
     * @note
     *        - 调用接口后，你会收到 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调歌曲播放状态。
     *        - 若音乐 ID 错误，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3023，play_state 为 4。
     *        - 若未进房，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3022，play_state 为 4。
     * @list 在线 KTV
     */
    
    virtual void stopMusic(const char *music_id) = 0;
    /**
     * 
     * @type api
     * @brief 设置音乐文件的起始播放位置。
     * @param music_id 音乐 ID。
     * @param position 音乐起始位置，单位为毫秒，取值小于音乐文件总时长。
     * @note
     *        - 调用本接口时音乐必须处于播放中状态。
     *        - 调用接口后，你会收到 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调歌曲播放状态。
     *        - 若音乐 ID 错误，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3023，play_state 为 4。
     *        - 若未进房，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3022，play_state 为 4。
     * @list 在线 KTV
     */
    
    virtual void seekMusic(const char *music_id, int position) = 0;
    /**
     * 
     * @type api
     * @brief 设置歌曲播放音量，只能在开始播放后进行设置。
     * @param music_id 音乐 ID。
     * @param volume 歌曲播放音量，调节范围：[0,400]。 <br>
     *        - 0：静音。
     *        - 100：原始音量。
     *        - 400: 原始音量的 4 倍(自带溢出保护)。
     * @note
     *        - 调用本接口时音乐必须处于播放中状态。
     *        - 若设置的音量大于 400，则按最大值 400 进行调整；若设置的音量小于 0，则按最小值 0 进行调整。
     *        - 若音乐 ID 错误，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3023，play_state 为 4。
     *        - 若未进房，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3022，play_state 为 4。
     * @list 在线 KTV
     */
    
    virtual void setMusicVolume(const char *music_id, int volume) = 0;
    /**
     * 
     * @type api
     * @brief 切换歌曲原唱伴唱。
     * @param music_id 音乐 ID。
     * @note 调用本接口时音乐必须处于播放中状态。
     * @list 在线 KTV
     */
    
    virtual void switchAudioTrackType(const char *music_id) = 0;
    /**
     * 
     * @type api
     * @brief 对播放中的音乐设置升降调信息。
     * @param music_id 音乐 ID。
     * @param pitch 相对于音乐文件原始音调的升高/降低值，取值范围 [-12，12]，默认值为 0，即不做调整。 <br>
     *              取值范围内每相邻两个值的音高距离相差半音，正值表示升调，负值表示降调，设置的绝对值越大表示音调升高或降低越多。
     * @note
     *        - 调用本接口时音乐必须处于播放中状态。
     *        - 若设置的 pitch 大于 12，则按最大值 12 进行调整；若设置的 pitch 小于 –12，则按最小值 –12 进行调整。
     *        - 若音乐 ID 错误，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3023，play_state 为 4。
     *        - 若未进房，会触发 onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} 回调，error_code 为 –3022，play_state 为 4。
     * @list 在线 KTV
     */
    
    virtual void setMusicPitch(const char* music_id, int pitch) = 0;

};
/**
 * 
 * @type api
 * @brief KTV 管理接口。
 * @list Online Karaoke
 */

class IKTVManager {
public:
    /**
     * 
     * @hidden constructor/destructor
     */
    
    virtual ~IKTVManager() = default;
    /**
     * 
     * @type api
     * @brief 设置歌曲文件最大占用的本地缓存。
     * @param max_cache_size_mb 本地缓存，单位 MB。 <br>
     *        设置值小于等于 0 时，使用默认值 1024 MB。
     * @list 在线 KTV
     */
    
    virtual void setMaxCacheSize(int max_cache_size_mb) = 0;
    /**
     * 
     * @type api
     * @brief 设置 KTV 回调接口。
     * @param ktv_manager_event_handler KTV 回调类，参看 IKTVManagerEventHandler{@link #IKTVManagerEventHandler}。
     * @list 在线 KTV
     */
    
    virtual void setKTVManagerEventHandler(IKTVManagerEventHandler *ktv_manager_event_handler) = 0;
    /**
     * 
     * @type api
     * @brief 获取歌曲列表。
     * @param page_num 页码，默认从 1 开始。
     * @param page_size 每页显示歌曲的最大数量，取值范围 [1,99]。
     * @param filters 歌曲过滤方式，参看 MusicFilterType{@link #MusicFilterType}。
     * @note 调用接口后，你会收到 onMusicListResult{@link #IKTVManagerEventHandler#onMusicListResult} 回调歌曲列表。
     * @list 在线 KTV
     */
    
    virtual void getMusicList(int page_num, int page_size,  int filters) = 0;
    /**
     * 
     * @type api
     * @brief 根据关键词搜索歌曲。
     * @param key_word 关键词，字符串长度最大为 20 个字符。
     * @param page_num 页码，默认从 1 开始。
     * @param page_size 每页显示歌曲的最大数量，取值范围 [1,99]。
     * @param filters 歌曲过滤方式，参看 MusicFilterType{@link #MusicFilterType}。
     * @note 调用接口后，你会收到 onSearchMusicResult{@link #IKTVManagerEventHandler#onSearchMusicResult} 回调歌曲列表。
     * @list 在线 KTV
     */
    
    virtual void searchMusic(const char* key_word, int page_num, int page_size, int filters) = 0;
    /**
     * 
     * @type api
     * @brief 指定歌曲榜单，并获取其歌曲列表。
     * @param custom_hotlist_id 榜单 ID。<br>
     *                          默认榜单 ID 有：`ContentCenter` 和 `Project`，分别是火山内容中心热歌榜和项目热歌榜。如果你需要其他榜单，请联系技术支持人员。
     * @param id_num 榜单 ID 的个数。
     * @param filters 歌曲过滤方式，参看 MusicFilterType{@link #MusicFilterType}。
     * @note 调用接口后，你会收到 onHotMusicResult{@link #IKTVManagerEventHandler#onHotMusicResult} 回调歌曲列表。
     * @list 在线 KTV
     */
    
    virtual void getHotMusic(const char** custom_hotlist_id, int id_num, int filters) = 0;
    /**
     * 
     * @type api
     * @brief 获取音乐详细信息。
     * @param music_id 音乐 ID。
     * @note 调用接口后，你会收到 onMusicDetailResult{@link #IKTVManagerEventHandler#onMusicDetailResult} 回调。
     * @list 在线 KTV
     */
    
    virtual void getMusicDetail(const char* music_id) = 0;
    /**
     * 
     * @type api
     * @brief 下载音乐。
     * @param music_id 音乐 ID。
     * @return 下载任务 ID。
     * @note
     *       - 若音乐下载成功，你会收到 onDownloadSuccess{@link #IKTVManagerEventHandler#onDownloadSuccess} 回调。
     *       - 若音乐下载失败，你会收到 onDownloadFailed{@link #IKTVManagerEventHandler#onDownloadFailed} 回调。
     *       - 音乐下载进度更新时，你会收到 onDownloadMusicProgress{@link #IKTVManagerEventHandler#onDownloadMusicProgress} 回调。
     * @list 在线 KTV
     */
    
    virtual int downloadMusic(const char* music_id) = 0;
    /**
     * 
     * @type api
     * @brief 下载歌词。
     * @param music_id 音乐 ID。
     * @param type 歌词文件类型，参看 DownloadLyricType{@link #DownloadLyricType}。
     * @return 下载任务 ID。
     * @note
     *       - 若歌词下载成功，你会收到 onDownloadSuccess{@link #IKTVManagerEventHandler#onDownloadSuccess} 回调。
     *       - 若歌词下载失败，你会收到 onDownloadFailed{@link #IKTVManagerEventHandler#onDownloadFailed} 回调。
     * @list 在线 KTV
     */
    
    virtual int downloadLyric(const char* music_id, DownloadLyricType type) = 0;
    /**
     * 
     * @type api
     * @brief 下载 MIDI 文件。
     * @param music_id 音乐 ID。
     * @return 下载任务 ID。
     * @note
     *       - 若文件下载成功，你会收到 onDownloadSuccess{@link #IKTVManagerEventHandler#onDownloadSuccess} 回调。
     *       - 若文件下载失败，你会收到 onDownloadFailed{@link #IKTVManagerEventHandler#onDownloadFailed} 回调。
     * @list 在线 KTV
     */
    
    virtual int downloadMidi(const char* music_id) = 0;
    /**
     * 
     * @type api
     * @brief 取消下载任务。
     * @param download_id 下载任务 ID。
     * @list 在线 KTV
     */
    
    virtual void cancelDownload(int download_id) = 0;
    /**
     * 
     * @type api
     * @brief 清除当前音乐缓存文件，包括音乐音频和歌词。
     * @list 在线 KTV
     */
    
    virtual void clearCache() = 0;
    /**
     * 
     * @type api
     * @brief 获取 KTV 播放器。
     * @return KTV 播放器接口，参看 IKTVPlayer{@link #IKTVPlayer}。
     * @list 在线 KTV
     */
    
    virtual IKTVPlayer *getKTVPlayer() = 0;

};

} 