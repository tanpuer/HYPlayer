//
// Created by templechen on 2019-09-06.
//

#ifndef HYPLAYER_GIFCREATOR_H
#define HYPLAYER_GIFCREATOR_H


#include "IAVFrameCreator.h"

extern "C" {
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};

class GifCreator : public IAVFrameCreator {

public:

    virtual AVFrame *readFrame(const char *path, int index = 0);

    virtual void releaseFrame();

    ~GifCreator();

private:

    AVFormatContext *ic;
    AVCodecContext *codecContext;
    SwsContext *img_convert_ctx;
    unsigned char *out_buffer;
    AVPacket *pkt;

};


#endif //HYPLAYER_GIFCREATOR_H
