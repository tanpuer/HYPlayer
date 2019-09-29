//
// Created by templechen on 2019-09-27.
//

#include <base/native_log.h>
#include "LiveEncoder.h"

LiveEncoder::LiveEncoder(JavaVM *vm) {
    this->vm = vm;
}

LiveEncoder::~LiveEncoder() {

}

void LiveEncoder::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kLiveEncoderStart: {
            vm->AttachCurrentThread(&env, nullptr);
            encoder = new FFVideoEncoder();
            encoder->InitEncoder("sdcard/trailer_test.mp4", 720, 1280);
            encoder->EncodeStart();
            break;
        }
        case kLiveEncoderDoFrame: {
            auto *data = static_cast<CameraNV21Data *>(msg->obj);
            encoder->EncoderNV21Buffer(data->data, data->width, data->height, data->pts * 1000000);
            delete data;
            break;
        }
        case kLiveEncoderStop: {
            vm->DetachCurrentThread();
            encoder->EncoderEnd();
            break;
        }
        default: {
            ALOGE("unknown type for LiveEncoder");
            break;
        }
    }
}

void LiveEncoder::pthreadExit() {
    Looper::pthreadExit();
}
