//
// Created by templechen on 2019-08-20.
//

#ifndef HYPLAYER_SLPLAYERLOOPER_H
#define HYPLAYER_SLPLAYERLOOPER_H


#include "../base/Looper.h"
#include "../decode/circle_av_frame_queue.h"
#include "IAudioPlayer.h"

class SLPlayerLooper : public Looper {

public:

    enum {
        kMsgSLPlayerCreated,
        kMsgSLPlayerStart,
        kMsgSLPlayerPause,
        kMsgSLPlayerSeek,
        kMsgSLPlayerRelease
    };

    SLPlayerLooper(circle_av_frame_queue *frameQueue);

    virtual ~SLPlayerLooper();

    void handleMessage(LooperMessage *msg) override;

    long getCurrentPos();

    void pthreadExit() override;

private:

    circle_av_frame_queue *frameQueue;

    IAudioPlayer *slAudioPlayer;

};


#endif //HYPLAYER_SLPLAYERLOOPER_H
