//
// Created by templechen on 2019-08-20.
//

#ifndef HYPLAYER_SLAUDIOPLAYER_H
#define HYPLAYER_SLAUDIOPLAYER_H


#include "../decode/circle_av_frame_queue.h"
#include "IAudioPlayer.h"

class SLAudioPlayer : public IAudioPlayer{

public:

    SLAudioPlayer(circle_av_frame_queue *frameQueue);

    ~SLAudioPlayer();

    void init();

    void start();

    void pause();

    void seek();

    void release();

    void call(void *bufq);

    void reset();

private:

    unsigned char* buf = 0;

    circle_av_frame_queue *frameQueue;
};


#endif //HYPLAYER_SLAUDIOPLAYER_H
