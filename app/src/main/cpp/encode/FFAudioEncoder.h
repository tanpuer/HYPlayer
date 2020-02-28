//
// Created by templechen on 2019-11-25.
//

#ifndef HYPLAYER_FFAUDIOENCODER_H
#define HYPLAYER_FFAUDIOENCODER_H


extern "C" {
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
};

class FFAudioEncoder {

public:

    FFAudioEncoder(const char *path);

    void encoderBuffer(short *buffer, int size);

    void encodeEnd();

    void release();

    void EncodeFrame(AVCodecContext *pCodecCtx, AVFrame *audioFrame);

private:

    const char *path;

    AVFormatContext *pFormatCtx = nullptr;
    AVOutputFormat *outputFormat = nullptr;
    AVCodecContext *pCodecCtx = nullptr;
    AVCodec *pCodec;
    AVFrame *audioFrame = nullptr;
    AVStream *pStream = nullptr;
    AVPacket audioPacket;
    int bufferSize = 0;
    uint8_t *audioBuffer = nullptr;
    SwrContext *swr = nullptr;

};


#endif //HYPLAYER_FFAUDIOENCODER_H
