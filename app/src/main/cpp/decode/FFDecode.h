//
// Created by templechen on 2019-08-19.
//

#ifndef HYPLAYER_FFDECODE_H
#define HYPLAYER_FFDECODE_H


#include "IDecode.h"

extern "C" {
#include "libswresample/swresample.h"
};

class FFDecode : public IDecode{

public:

    virtual ~FFDecode();

    virtual bool init();

    virtual bool start();

    virtual bool pause();

    virtual void release();

private:

    AVCodecParameters *parameters;

    AVCodec *codec;

    AVCodecContext *codecContext;

    SwrContext *swrContext;

};


#endif //HYPLAYER_FFDECODE_H
