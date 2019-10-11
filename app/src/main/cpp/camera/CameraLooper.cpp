//
// Created by templechen on 2019-10-09.
//

#include <base/native_log.h>
#include "CameraLooper.h"

CameraLooper::CameraLooper(ANativeWindow *nativeWindow, JavaVM *vm, jobject javaCameraView) {
    this->nativeWindow = nativeWindow;
    this->vm = vm;
    this->javaCameraView = javaCameraView;
}

CameraLooper::~CameraLooper() {

}

void CameraLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgCameraViewCreated: {
            renderer = new CameraRenderer(vm, javaCameraView);
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
