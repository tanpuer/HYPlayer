//
// Created by templechen on 2019-04-28.
//

#include "GLVideoLooper.h"
extern "C" {
#include <libavutil/frame.h>
}

GLVideoLooper::GLVideoLooper(circle_av_frame_queue *frameQueue) {
    this->frameQueue = frameQueue;
    renderer = new GLVideoRenderer(frameQueue);
    destroyed = false;
}

GLVideoLooper::~GLVideoLooper() {
    delete renderer;
    renderer = nullptr;
}

void GLVideoLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgSurfaceCreated: {
            if (renderer != nullptr) {
                renderer->surfaceCreated(static_cast<ANativeWindow *>(msg->obj));
            }
            break;
        }
        case kMsgSurfaceChanged: {
            if (renderer != nullptr) {
                renderer->surfaceChanged(msg->arg1, msg->arg2);
            }
            break;
        }
        case kMsgSurfaceDestroyed: {
            destroyed = true;
            if (renderer != nullptr) {
                renderer->surfaceDestroyed();
            }
            break;
        }
        case kMsgSurfaceDoFrame: {
            if (renderer != nullptr && !destroyed) {
                renderer->surfaceDoFrame();
            }
            break;
        }
        default:
            break;
    }
}

