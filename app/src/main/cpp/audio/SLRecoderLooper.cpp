//
// Created by templechen on 2019-11-25.
//

#include <base/native_log.h>
#include "SLRecoderLooper.h"

void SLRecoderLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgSLRecoderCreated: {
            audioRecoder = new SLAudioRecoder(static_cast<const char *>(msg->obj));
            break;
        }
        case kMsgSLRecoderStart: {
            if (audioRecoder != nullptr) {
                audioRecoder->start();
            }
            break;
        }
        case kMsgSLRecoderPause: {
            if (audioRecoder != nullptr) {
                audioRecoder->pause();
            }
            break;
        }
        case kMsgSLRecoderRelease: {
            if (audioRecoder != nullptr) {
                audioRecoder->release();
                delete audioRecoder;
            }
            break;
        }
        default: {
            ALOGD("unknown msg type for SLAudioRecoder");
            break;
        }
    }
}

void SLRecoderLooper::pthreadExit() {
    Looper::pthreadExit();
}
