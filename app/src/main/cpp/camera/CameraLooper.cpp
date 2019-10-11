//
// Created by templechen on 2019-10-09.
//

#include <base/native_log.h>
#include "CameraLooper.h"

CameraLooper::CameraLooper(ANativeWindow *nativeWindow) {
    this->nativeWindow = nativeWindow;
}

CameraLooper::~CameraLooper() {

}

void CameraLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgCameraViewCreated: {
            renderer = new CameraRenderer();
            renderer->cameraViewCreated(nativeWindow);
            break;
        }
        case kMsgCameraViewChanged: {
            renderer->cameraViewChanged(msg->arg1, msg->arg2);
            break;
        }
        case kMsgCameraViewDestroyed: {
            renderer->cameraViewDestroyed();
            break;
        }
        case kMsgCameraViewDoFrame: {
            renderer->cameraViewDoFrame();
            break;
        }
        default: {
            ALOGE("unknown type for CameraLooper");
            break;
        }
    }
}

void CameraLooper::pthreadExit() {
    Looper::pthreadExit();
}
