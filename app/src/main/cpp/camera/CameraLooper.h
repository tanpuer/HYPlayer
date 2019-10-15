//
// Created by templechen on 2019-10-09.
//

#ifndef HYPLAYER_CAMERALOOPER_H
#define HYPLAYER_CAMERALOOPER_H


#include <android/native_window.h>
#include <jni.h>
#include "../base/Looper.h"
#include "CameraRenderer.h"

class CameraLooper : public Looper {

public:

    enum {
        kMsgCameraViewCreated,
        kMsgCameraViewChanged,
        kMsgCameraViewDestroyed,
        kMsgCameraViewDoFrame,
        kMsgCameraSizeChanged
    };

    CameraLooper(ANativeWindow *nativeWindow, JavaVM *vm, jobject javaCameraView);

    ~CameraLooper();

    void handleMessage(LooperMessage *msg) override;

    void pthreadExit() override;

private:

    ANativeWindow *nativeWindow;

    CameraRenderer *renderer;

    JavaVM *vm;

    jobject javaCameraView;

};


#endif //HYPLAYER_CAMERALOOPER_H
