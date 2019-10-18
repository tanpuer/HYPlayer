//
// Created by templechen on 2019-08-19.
//

#ifndef HYPLAYER_DECODELOOPER_H
#define HYPLAYER_DECODELOOPER_H


#include "../base/Looper.h"
#include "IDecode.h"
#include "circle_av_frame_queue.h"

class DecodeLooper : public Looper {

public:

    enum {
        kMsgDecodeCreated,
        kMsgDecodeSeek,
        kMsgDecodeRelease
    };

    DecodeLooper(circle_av_frame_queue *frameQueue, circle_av_packet_queue *packetQueue,
                 bool isAudio = true);

    ~DecodeLooper();

    void handleMessage(LooperMessage *msg) override;

    IDecode *decode;

    void pthreadExit() override;

private:

    circle_av_frame_queue *frameQueue;

    circle_av_packet_queue *packetQueue;

    bool isAudio;

};


#endif //HYPLAYER_DECODELOOPER_H
