//
// Created by templechen on 2019-08-19.
//

#ifndef HYPLAYER_CIRCLE_AV_FRAME_QUEUE_H
#define HYPLAYER_CIRCLE_AV_FRAME_QUEUE_H

#include "../base/AVFrameData.h"

#define DEFAULT_AV_FRAME_SIZE 100

struct AVFrameStruct {
    AVFrameData *data;
    AVFrameStruct *next;
};

class circle_av_frame_queue {

public:

    circle_av_frame_queue();

    ~circle_av_frame_queue();

    void push(AVFrameData *frame);

    AVFrameData *pull();

    int64_t pullHeadFramePts();

private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    AVFrameStruct *tail;
    AVFrameStruct *head;
    AVFrameStruct *pullCursor;
    AVFrameStruct *pushCursor;

    int currSize = 0;

    bool decodeStarted = false;

};


#endif //HYPLAYER_CIRCLE_AV_FRAME_QUEUE_H
