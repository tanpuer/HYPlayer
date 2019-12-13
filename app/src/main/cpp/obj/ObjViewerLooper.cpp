//
// Created by templechen on 2019-11-26.
//

#include <base/native_log.h>
#include "ObjViewerLooper.h"

ObjViewerLooper::ObjViewerLooper(ANativeWindow *window) {
    this->window = window;
}

ObjViewerLooper::~ObjViewerLooper() {

}

void ObjViewerLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgObjViewerCreated: {
            renderer = new ObjViewerRenderer();
            renderer->objViewerCreated(window);
            break;
        }
        case kMsgObjViewerChanged: {
            if (renderer != nullptr) {
                renderer->objViewerChanged(msg->arg1, msg->arg2);
            }
            break;
        }
        case kMsgObjViewerDestroyed: {
            if (renderer != nullptr) {
                renderer->objViewerDestroyed();
            }
            quit();
            break;
        }
        case kMsgObJViewerDoFrame: {
            if (renderer != nullptr) {
                renderer->objViewerDoFrame();
            }
            break;
        }
        case kMsgObjViewScroll: {
            if (renderer != nullptr) {
                renderer->objViewerOnScroll(msg->arg1, msg->arg2);
            }
            break;
        }
        default: {
            ALOGE("unknown type for ObjViewerLooper");
            break;
        }

    }
}

void ObjViewerLooper::pthreadExit() {
    Looper::pthreadExit();
}

void ObjViewerLooper::setObjScroll(int scrollX, int scrollY) {
    if (renderer != nullptr) {
        renderer->objViewerOnScroll(scrollX, scrollY);
    }
}

void ObjViewerLooper::setObjScale(float scale) {
    if (renderer != nullptr) {
        renderer->objViewerOnScale(scale);
    }
}
