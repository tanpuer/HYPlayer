//
// Created by templechen on 2019-08-19.
//

#include "DecodeLooper.h"
#include "../base/native_log.h"
#include "FFDecode.h"
#include "FFVideoDecode.h"

DecodeLooper::DecodeLooper(circle_av_frame_queue *frameQueue, circle_av_packet_queue *packetQueue,
                           bool isAudio, bool usingMediaCodec) {
    this->frameQueue = frameQueue;
    this->packetQueue = packetQueue;
    this->isAudio = isAudio;
    this->usingMediaCodec = usingMediaCodec;
}

DecodeLooper::~DecodeLooper() {

}

void DecodeLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgDecodeCreated: {
            if (isAudio) {
                decode = new FFDecode();
            } else {
                decode = new FFVideoDecode(usingMediaCodec);
            }
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
