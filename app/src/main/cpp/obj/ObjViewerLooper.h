//
// Created by templechen on 2019-11-26.
//

#ifndef HYPLAYER_OBJVIEWERLOOPER_H
#define HYPLAYER_OBJVIEWERLOOPER_H


#include <base/Looper.h>
#include <android/native_window.h>
#include "ObjViewerRenderer.h"

class ObjViewerLooper : public Looper{

public:

    enum {
        kMsgObjViewerCreated,
        kMsgObjViewerChanged,
        kMsgObjViewerDestroyed,
        kMsgObJViewerDoFrame,
        kMsgObjViewScroll,
    };

    ObjViewerLooper(ANativeWindow *window);

    ~ObjViewerLooper();

    void handleMessage(LooperMessage *msg) override;

    void pthreadExit() override;

    void setObjScroll(int scrollX, int scrollY);

private:

    ANativeWindow *window;

    ObjViewerRenderer *renderer = nullptr;

};


#endif //HYPLAYER_OBJVIEWERLOOPER_H
