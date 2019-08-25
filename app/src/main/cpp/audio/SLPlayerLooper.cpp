//
// Created by templechen on 2019-08-20.
//

#include "SLPlayerLooper.h"
#include "../base/native_log.h"
#include "SLAudioPlayer.h"

SLPlayerLooper::SLPlayerLooper(circle_av_frame_queue *frameQueue) {
    this->frameQueue = frameQueue;
}

SLPlayerLooper::~SLPlayerLooper() {

}

void SLPlayerLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgSLPlayerCreated: {
            slAudioPlayer = new SLAudioPlayer(frameQueue);
            break;
        }
        case kMsgSLPlayerStart: {
            slAudioPlayer->start();
            break;
        }
        case kMsgSLPlayerPause: {
            slAudioPlayer->pause();
            break;
        }
        case kMsgSLPlayerSeek: {
            break;
        }
        case kMsgSLPlayerRelease: {
            slAudioPlayer->release();
            quit();
            break;
        }
        default: {
            ALOGE("unknown msg in SLPlayerLooper!")
            break;
        }
    }
}

long SLPlayerLooper::getCurrentPos() {
    if (slAudioPlayer != nullptr) {
        return slAudioPlayer->currentPos;
    }
    return 0;
}

void SLPlayerLooper::pthreadExit() {
    delete this;
}
