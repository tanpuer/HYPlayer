//
// Created by templechen on 2019-09-09.
//

#ifndef HYPLAYER_VIDEOENCODER_H
#define HYPLAYER_VIDEOENCODER_H

#include <cstdio>
#include "media/NdkMediaCodec.h"
#include "media/NdkMediaExtractor.h"
#include "media/NdkMediaMuxer.h"

class VideoEncoder {

public:

    VideoEncoder();

    ~VideoEncoder();

    void drainEncoderWithNoTimeOut(bool endOfStream);

    void release();

    ANativeWindow *nativeWindow = nullptr;

private:

    FILE *file;
    AMediaCodec *codec;
    AMediaFormat *format;
    AMediaCodecBufferInfo *bufferInfo;
    AMediaMuxer *muxer;

    int trackIndex = -1;
};


#endif //HYPLAYER_VIDEOENCODER_H
