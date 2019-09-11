//
// Created by templechen on 2019-09-10.
//

#ifndef HYPLAYER_IVIDEOENCODER_H
#define HYPLAYER_IVIDEOENCODER_H


extern "C" {
#include <libavutil/pixfmt.h>
};

class IVideoEncoder {

public:

    virtual void InitEncoder(const char *path, int width, int height) = 0;

    virtual void EncodeStart() = 0;

    virtual void EncoderBuffer(unsigned char *buffer, long long pts) = 0;

    virtual void EncoderEnd() = 0;
};


#endif //HYPLAYER_IVIDEOENCODER_H
