//
// Created by templechen on 2019-09-06.
//

#ifndef HYPLAYER_VIDEOCREATOR_H
#define HYPLAYER_VIDEOCREATOR_H


extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

#include "vector"
#include "IAVFrameCreator.h"

class VideoCreator : public IAVFrameCreator {

public:

    VideoCreator(const char *path);

    virtual ~VideoCreator();

    virtual AVFrame *readFrame(int index = 0);

    virtual void releaseFrame();

    static void *trampoline(void *p);

private:

    void startEncode();

    AVFormatContext *ic = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVPacket *pkt = nullptr;
    std::vector<AVFrame *> frameList;
    long long totalMs;
    int size;
    pthread_t worker_thread;
    const char *path;
    int videoIndex;
    double timeBase;
};


#endif //HYPLAYER_VIDEOCREATOR_H
