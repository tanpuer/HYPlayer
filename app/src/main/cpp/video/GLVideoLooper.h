//
// Created by templechen on 2019-04-28.
//

#ifndef FFMPEG_PLAYER_GL_LOOPER_H
#define FFMPEG_PLAYER_GL_LOOPER_H


#include <decode/circle_av_frame_queue.h>
#include "GLVideoRenderer.h"
#include "../base/Looper.h"

class GLVideoLooper : public Looper {

public:

    enum {
        kMsgSurfaceCreated,
        kMsgSurfaceChanged,
        kMsgSurfaceDestroyed,
        kMsgSurfaceDoFrame
    };

    GLVideoLooper(circle_av_frame_queue *frameQueue);

    virtual ~GLVideoLooper();

    virtual void handleMessage(LooperMessage *msg) override;

private:
    GLVideoRenderer *renderer;

    bool destroyed;

    circle_av_frame_queue *frameQueue;
};

#endif //FFMPEG_PLAYER_GL_LOOPER_H
