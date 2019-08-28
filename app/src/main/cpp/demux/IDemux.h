//
// Created by templechen on 2019-08-09.
//

#ifndef HYPLAYER_IDEMUX_H
#define HYPLAYER_IDEMUX_H

#include "circle_av_packet_queue.h"

class IDemux {

public:

    virtual bool init(const char *url) = 0;

    virtual bool start() = 0;

    virtual bool pause() = 0;

    virtual bool seek(long pos) = 0;

    virtual void release() = 0;

    bool isDemuxing = false;

    bool isOver = false;

    circle_av_packet_queue *packetQueue;

    int64_t totalDuration;

};


#endif //HYPLAYER_IDEMUX_H
