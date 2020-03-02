//
// Created by templechen on 2020/3/2.
//

#ifndef HYPLAYER_AACENCODER_H
#define HYPLAYER_AACENCODER_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>

}
class AACEncoder {

private:

    int index = 0;
    int bufferSize = 0;

    AVFormatContext *pFormatCtx = NULL;
    AVOutputFormat *fmt = NULL;
    AVStream *audioStream = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;

    uint8_t *audioBuffer = NULL;
    AVFrame *audioFrame = NULL;
    AVPacket audioPacket;

    SwrContext *swr;

    int EncodeFrame(AVCodecContext *pCodecCtx, AVFrame *pFrame);

public:

    int EncodeStart(const char *aacPath);

    int EncodeBuffer(const unsigned char *pcmBuffer, int length);

    int EncodeStop();

};


#endif //HYPLAYER_AACENCODER_H
