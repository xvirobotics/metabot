/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief ByteRTCKTVPlayer
*/

@interface ByteRTCKTVPlayer ()

- (instancetype)initWithKTVPlayer:(bytertc::IKTVPlayer *)ktvPlayer;

- (void)destroyResourceInternal;

@end
