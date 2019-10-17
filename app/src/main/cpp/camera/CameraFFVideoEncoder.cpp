//
// Created by templechen on 2019-10-15.
//

#include <base/native_log.h>
#include "CameraFFVideoEncoder.h"

CameraFFVideoEncoder::CameraFFVideoEncoder() {

}

CameraFFVideoEncoder::~CameraFFVideoEncoder() {

}

void CameraFFVideoEncoder::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgCameraEncodeStart: {
            videoEncoder = new FFVideoEncoder();
            videoEncoder->InitEncoder("sdcard/trailer_test.mp4", 720, 1280);
            videoEncoder->EncodeStart();
            break;
        }
        case kMsgCameraEncoderFrame: {
            videoEncoder->EncodeYUV420Buffer(reinterpret_cast<unsigned char *>(msg->obj), msg->arg1,
                                             msg->arg2);
            break;
        }
        case kMsgCameraEncoderEnd: {
            videoEncoder->EncoderEnd();
            break;
        }
        default: {
            ALOGD("unknown type for CameraFFVideoEncoder");
            break;
        }
    }
}

void CameraFFVideoEncoder::pthreadExit() {
    Looper::pthreadExit();
    delete videoEncoder;
    delete this;
}
