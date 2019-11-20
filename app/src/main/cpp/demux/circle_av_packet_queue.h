//
// Created by templechen on 2019-08-19.
//

#ifndef HYPLAYER_CIRCLE_AV_PACKET_QUEUE_H
#define HYPLAYER_CIRCLE_AV_PACKET_QUEUE_H


#include <sys/types.h>
#include "../base/AVPacketData.h"

#define DEFAULT_AV_PACKET_SIZE 100

struct AVPacketStruct {
    AVPacketData *data;
    AVPacketStruct *next;
};

class circle_av_packet_queue {

public:

    circle_av_packet_queue();

    virtual ~circle_av_packet_queue();

    void push(AVPacketData *data);

    AVPacketData* pull();

    AVPacketData* pop();

private:

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    AVPacketStruct *tail;
    AVPacketStruct *head;
    AVPacketStruct *pullCursor;
    AVPacketStruct *pushCursor;

    int currSize = 0;

    bool demuxStarted = false;
};


#endif //HYPLAYER_CIRCLE_AV_PACKET_QUEUE_H
