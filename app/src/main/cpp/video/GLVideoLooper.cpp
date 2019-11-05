//
// Created by templechen on 2019-04-28.
//

#include "GLVideoLooper.h"

extern "C" {
#include <libavutil/frame.h>
}

GLVideoLooper::GLVideoLooper(circle_av_frame_queue *frameQueue) {
    this->frameQueue = frameQueue;
    glVideoPlayer = new GLVideoPlayer(frameQueue);
    destroyed = false;
}

GLVideoLooper::~GLVideoLooper() {
    delete glVideoPlayer;
    glVideoPlayer = nullptr;
}

void GLVideoLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgSurfaceCreated: {
            if (glVideoPlayer != nullptr) {
                glVideoPlayer->surfaceCreated(static_cast<ANativeWindow *>(msg->obj));
            }
            destroyed = false;
            break;
        }
        case kMsgSurfaceChanged: {
            if (glVideoPlayer != nullptr && !destroyed) {
                glVideoPlayer->surfaceChanged(msg->arg1, msg->arg2);
            }
            break;
        }
        case kMsgSurfaceDestroyed: {
            if (glVideoPlayer != nullptr && !destroyed) {
                glVideoPlayer->surfaceDestroyed();
                quit();
            }
            destroyed = true;
            break;
        }
        case kMsgSurfaceDoFrame: {
            if (glVideoPlayer != nullptr && !destroyed) {
                glVideoPlayer->surfaceDoFrame();
            }
            break;
        }
        case kMsgSurfaceStart: {
            if (glVideoPlayer != nullptr && !destroyed) {
                glVideoPlayer->start();
            }
            break;
        }
        case kMsgSurfacePause: {
            if (glVideoPlayer != nullptr && !destroyed) {
                glVideoPlayer->pause();
            }
            break;
        }
        case kMsgSurfaceSeek: {
            if (glVideoPlayer != nullptr && !destroyed) {
                glVideoPlayer->seek();
            }
            break;
        }
        default:
            break;
    }
}

long GLVideoLooper::getCurrentPos() {
    if (glVideoPlayer != nullptr) {
        return glVideoPlayer->getCurrentPos();
    }
    return 0;
}

