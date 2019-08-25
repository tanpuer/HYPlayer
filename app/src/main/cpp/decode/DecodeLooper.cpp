//
// Created by templechen on 2019-08-19.
//

#include "DecodeLooper.h"
#include "../base/native_log.h"
#include "FFDecode.h"

DecodeLooper::DecodeLooper(circle_av_frame_queue *frameQueue, circle_av_packet_queue *packetQueue) {
    this->frameQueue = frameQueue;
    this->packetQueue = packetQueue;
}

DecodeLooper::~DecodeLooper() {

}

void DecodeLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgDecodeCreated: {
            decode = new FFDecode();
            decode->packetQueue = packetQueue;
            decode->frameQueue = frameQueue;
            decode->init();
            decode->isDecoding = true;
            decode->start();
            break;
        }
        case kMsgDecodeSeek: {
            break;
        }
        case kMsgDecodeRelease: {
            decode->release();
            quit();
            break;
        }
        default: {
            ALOGE("unknown decode message type!")
        }
    }
}

void DecodeLooper::pthreadExit() {
    delete this;
}
