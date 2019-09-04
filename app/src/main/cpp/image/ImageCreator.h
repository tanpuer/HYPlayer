//
// Created by templechen on 2019-09-02.
//

#ifndef HYPLAYER_IMAGECREATOR_H
#define HYPLAYER_IMAGECREATOR_H



extern "C" {
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

class ImageCreator {

public:

    AVFrame *readImage(const char *path);

    void releaseImage();

private:

    AVFrame *pFrameYUV;
    AVFrame *frame;
    AVPacket *pkt;
    AVFormatContext *ic;
    AVCodecContext *codecContext;
    unsigned char *out_buffer;
};


#endif //HYPLAYER_IMAGECREATOR_H
