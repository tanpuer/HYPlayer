//
// Created by templechen on 2019-09-06.
//

#ifndef HYPLAYER_GIFCREATOR_H
#define HYPLAYER_GIFCREATOR_H


#include <base/Looper.h>
#include "IAVFrameCreator.h"

extern "C" {
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};

#include "vector"

class GifCreator : public IAVFrameCreator, public Looper{

public:

    GifCreator(const char *path);

    virtual AVFrame *readFrame(int index = 0);

    virtual void releaseFrame();

    ~GifCreator();

    void handleMessage(LooperMessage *msg) override;

    void pthreadExit() override;

private:

    enum {
        kMsgGifCreatorStart,
        kMsgGifCreatorStop
    };

    bool isDecoding = true;

    void startDecode();

    AVFormatContext *ic = nullptr;
    AVCodecContext *codecContext = nullptr;
    SwsContext *img_convert_ctx = nullptr;
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;
    std::vector<AVFrame*> frameList;
    std::vector<unsigned char *>bufferList;
    long long totalMs;
    int size;

    const char *path;
};


#endif //HYPLAYER_GIFCREATOR_H
