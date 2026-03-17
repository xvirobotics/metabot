/*
 * Copyright (c) 2022 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief ByteRTCKTVManager
*/

@interface ByteRTCKTVManager ()

- (instancetype)initWithKTVManager:(bytertc::IKTVManager *)ktvManager;

- (void)destroyResourceInternal;

@end
