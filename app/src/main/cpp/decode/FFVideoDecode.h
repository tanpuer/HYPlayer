//
// Created by templechen on 2019-10-18.
//

#ifndef HYPLAYER_FFVIDEODECODE_H
#define HYPLAYER_FFVIDEODECODE_H


#include "FFDecode.h"

class FFVideoDecode : public IDecode{

public:

    FFVideoDecode(bool usingMediaCodec);

    virtual ~FFVideoDecode();

    virtual bool init();

    virtual bool start();

    virtual bool pause();

    virtual void release();

private:

    AVCodecParameters *parameters = nullptr;

    AVCodec *codec = nullptr;

    AVCodecContext *codecContext = nullptr;

    double timeBase;

    bool usingMediaCodec;

};


#endif //HYPLAYER_FFVIDEODECODE_H
