//
// Created by templechen on 2019-09-02.
//

#ifndef HYPLAYER_IMAGECREATOR_H
#define HYPLAYER_IMAGECREATOR_H


#include "IAVFrameCreator.h"

extern "C" {
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};

class ImageCreator : public IAVFrameCreator{

public:

    ImageCreator(const char *path);

    ~ImageCreator();

    virtual AVFrame *readFrame(int index = 0);

    virtual void releaseFrame();

private:

    AVFrame *pFrameYUV = nullptr;
    AVFrame *frame = nullptr;
    AVPacket *pkt = nullptr;
    AVFormatContext *ic = nullptr;
    AVCodecContext *codecContext = nullptr;
    SwsContext *img_convert_ctx = nullptr;
    unsigned char *out_buffer = nullptr;
    const char *path;
};


#endif //HYPLAYER_IMAGECREATOR_H
