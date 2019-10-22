//
// Created by templechen on 2019-08-20.
//

#ifndef HYPLAYER_AUDIOPLAYER_H
#define HYPLAYER_AUDIOPLAYER_H

#include "../demux/circle_av_packet_queue.h"
#include "../decode/circle_av_frame_queue.h"
#include "../demux/DemuxLooper.h"
#include "../decode/DecodeLooper.h"
#include "../audio/SLPlayerLooper.h"

class AudioPlayer {

public:

    AudioPlayer(const char* path);

    virtual ~AudioPlayer();

    void start();

    void pause();

    void seek(long pos);

    void release();

    long getTotalDuration();

    long getCurrentDuration();

    void setLoop(bool loop);

private:

    const char *path;

    circle_av_packet_queue *packetQueue;
    circle_av_frame_queue *frameQueue;
    DemuxLooper *demuxLooper;
    DecodeLooper *decodeLooper;
    SLPlayerLooper *slPlayerLooper;

};


#endif //HYPLAYER_AUDIOPLAYER_H
