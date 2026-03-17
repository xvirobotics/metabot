/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room Internal API
*/

@interface RTSRoom ()

/**
 * 
 * @type api
 * @hidden 仅内部使用接口
 */

- (instancetype _Nonnull)initWithRtsRoom:(byterts::IRTSRoom *_Nonnull)rtsRoom roomId:(NSString *_Nonnull)roomId;

@end
