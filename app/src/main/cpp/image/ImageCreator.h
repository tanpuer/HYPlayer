//
// Created by templechen on 2019-09-02.
//

#ifndef HYPLAYER_IMAGECREATOR_H
#define HYPLAYER_IMAGECREATOR_H


extern "C" {
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};

class ImageCreator {

public:

    AVFrame *readImage(const char *path);

    void releaseImage();

    ~ImageCreator();

private:

    AVFrame *pFrameYUV;
    AVFrame *frame;
    AVPacket *pkt;
    AVFormatContext *ic;
    AVCodecContext *codecContext;
    SwsContext *img_convert_ctx;
    unsigned char *out_buffer;
};


#endif //HYPLAYER_IMAGECREATOR_H
