//
// Created by templechen on 2019-09-02.
//

#include "TemplateLooper.h"
#include "../base/native_log.h"

void TemplateLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgTemplateCreated: {
            renderer->templateCreated(nativeWindow);
            break;
        }
        case kMsgTemplateChanged: {
            renderer->templateChanged(msg->arg1, msg->arg2);
            break;
        }
        case kMsgTemplateDestroyed: {
            renderer->templateDestroyed();
            break;
        }
        case kMsgTemplateDoFrame: {
            renderer->templateDoFrame(reinterpret_cast<long>(msg->obj));
            break;
        }
        default: {
            ALOGE("TemplateLooper unknown type")
            return;
        }
    }
}

TemplateLooper::TemplateLooper(ANativeWindow *nativeWindow) {
    renderer = new TemplateRenderer();
    this->nativeWindow = nativeWindow;
}

TemplateLooper::~TemplateLooper() {
    delete renderer;
}
