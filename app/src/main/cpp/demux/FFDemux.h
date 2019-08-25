//
// Created by templechen on 2019-08-18.
//

#ifndef HYPLAYER_FFDEMUX_H
#define HYPLAYER_FFDEMUX_H


#include <sys/types.h>
#include "IDemux.h"
#include "circle_av_packet_queue.h"

extern "C" {
#include <libavformat/avformat.h>
};

class FFDemux : public IDemux {

public:

    virtual ~FFDemux();

    virtual bool init(const char *url);

    virtual bool start();

    virtual bool pause();

    virtual bool seek(long pos);

    virtual void release();

private:

    const char *url;

    AVFormatContext *ic;

    int audioStreamIndex = -1;

};


#endif //HYPLAYER_FFDEMUX_H
