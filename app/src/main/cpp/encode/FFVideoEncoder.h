//
// Created by templechen on 2019-09-10.
//

#ifndef HYPLAYER_FFVIDEOENCODER_H
#define HYPLAYER_FFVIDEOENCODER_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
};

#include "IVideoEncoder.h"

class FFVideoEncoder : public IVideoEncoder {

public:

    virtual void InitEncoder(const char *path, int width, int height);

    virtual void EncodeStart();

    virtual void EncoderBuffer(unsigned char *buffer, long long pts);

    virtual void EncoderNV21Buffer(unsigned char *buffer, int width, int height, long long pts);

    virtual void EncoderRGBABuffer(unsigned char *buffer, int width, int height);

    virtual void EncoderEnd();

private:

    int EncoderFrame(AVCodecContext *pCodecCtx, AVFrame *pFrame, AVPacket *avPacket);

    const char *mp4Path;
    int width;
    int height;

    AVPacket avPacket;
    AVFormatContext *pFormatCtx = nullptr;
    AVStream *pStream = nullptr;
    AVCodecContext *pCodecCtx = nullptr;
    AVCodec *pCodec = nullptr;
    uint8_t *pFrameBuffer = nullptr;
    AVFrame *pFrame = nullptr;
    SwsContext *swsContext = nullptr;

    const int frame_rate = 60;
    const int bit_rate = 2000000;
    const int gop_size = 100;

    FILE *fp_out;
};


#endif //HYPLAYER_FFVIDEOENCODER_H
