//
// Created by templechen on 2019-09-22.
//

#include <base/native_log.h>
#include "FlutterLooper.h"

FlutterLooper::FlutterLooper(ANativeWindow *window) {
    this->window = window;
    renderer = new FlutterRenderer();
}

FlutterLooper::~FlutterLooper() {

}

void FlutterLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgFlutterCreated: {
            renderer->flutterCreated(window);
            break;
        }
        case kMsgFlutterChanged: {
            renderer->flutterChanged(msg->arg1, msg->arg2);
            break;
        }
        case kMsgFlutterDestroyed: {
            renderer->flutterDestroyed();
            break;
        }
        case kMsgFlutterDoFrame: {
            renderer->flutterDoFrame(reinterpret_cast<long>(msg->obj));
            break;
        }
        default: {
            ALOGE("TemplateLooper unknown type")
            return;
        }
    }
}

void FlutterLooper::pthreadExit() {
    ALOGD("flutter thread exit");
    delete renderer;
    renderer = nullptr;
    window = nullptr;
}
