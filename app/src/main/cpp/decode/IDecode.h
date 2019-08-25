//
// Created by templechen on 2019-08-09.
//

#ifndef HYPLAYER_IDECODE_H
#define HYPLAYER_IDECODE_H


#include "../demux/circle_av_packet_queue.h"
#include "circle_av_frame_queue.h"

class IDecode {

public:

    virtual bool init() = 0;

    virtual bool start() = 0;

    virtual bool pause() = 0;

    virtual void release() = 0;

    circle_av_packet_queue *packetQueue;

    circle_av_frame_queue *frameQueue;

    bool isDecoding = false;

};


#endif //HYPLAYER_IDECODE_H
