//
// Created by templechen on 2019-09-22.
//

#ifndef HYPLAYER_FLUTTERLOOPER_H
#define HYPLAYER_FLUTTERLOOPER_H


#include <android/native_window.h>
#include "../base/Looper.h"
#include "FlutterRenderer.h"

class FlutterLooper : public Looper {

public:

    enum {
        kMsgFlutterCreated,
        kMsgFlutterChanged,
        kMsgFlutterDestroyed,
        kMsgFlutterDoFrame
    };

    FlutterLooper(ANativeWindow *window);

    ~FlutterLooper();

    void handleMessage(LooperMessage *msg) override;

    void pthreadExit() override;

private:

    ANativeWindow *window;

    FlutterRenderer *renderer;
};


#endif //HYPLAYER_FLUTTERLOOPER_H
