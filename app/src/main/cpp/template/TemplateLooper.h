//
// Created by templechen on 2019-09-02.
//

#ifndef HYPLAYER_TEMPLATELOOPER_H
#define HYPLAYER_TEMPLATELOOPER_H


#include <android/native_window.h>
#include "../base/Looper.h"
#include "TemplateRenderer.h"

class TemplateLooper : public Looper {

public:

    enum {
        kMsgTemplateCreated,
        kMsgTemplateChanged,
        kMsgTemplateDestroyed,
        kMsgTemplateDoFrame
    };

    TemplateLooper(ANativeWindow *nativeWindow);

    virtual ~TemplateLooper();

    void handleMessage(LooperMessage *msg) override;

    void pthreadExit() override;

private:

    ANativeWindow *nativeWindow;

    TemplateRenderer *renderer;

};


#endif //HYPLAYER_TEMPLATELOOPER_H
