//
// Created by templechen on 2019-08-18.
//

#ifndef HYPLAYER_DEMUXLOOPER_H
#define HYPLAYER_DEMUXLOOPER_H


#include "../base/Looper.h"
#include "IDemux.h"
#include "circle_av_packet_queue.h"

class DemuxLooper : public Looper {

public:

    enum {
        kMsgDemuxCreated,
        kMsgDemuxSeek,
        kMsgDemuxRelease
    };

    DemuxLooper(circle_av_packet_queue *queue, bool isAudio = true);

    virtual ~DemuxLooper();

    void handleMessage(LooperMessage *msg) override;

    long getTotalDuration();

    IDemux *demux;

    void pthreadExit() override;

private:

    circle_av_packet_queue *queue;

    bool isAudio;

};


#endif //HYPLAYER_DEMUXLOOPER_H
