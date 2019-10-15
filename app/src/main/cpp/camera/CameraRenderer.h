//
// Created by templechen on 2019-10-11.
//

#ifndef HYPLAYER_CAMERARENDERER_H
#define HYPLAYER_CAMERARENDERER_H


#include <android/native_window.h>
#include <egl/egl_core.h>
#include <egl/window_surface.h>
#include <jni.h>
#include "CameraBaseFilter.h"
#include "CameraFFVideoEncoder.h"

class CameraRenderer {

public:

    CameraRenderer(JavaVM *vm, jobject javaCameraView);

    ~CameraRenderer();

    void cameraViewCreated(ANativeWindow *nativeWindow);

    void cameraViewChanged(int width, int height);

    void cameraViewDestroyed();

    void cameraViewDoFrame();

    void cameraSizeChanged(int width, int height);

private:

    CameraBaseFilter *baseFilter;

    egl_core *eglCore;

    window_surface *windowSurface;

    JavaVM *vm;

    JNIEnv *env;

    jobject javaCameraView;

    jclass clazz;

    CameraFFVideoEncoder *cameraFfVideoEncoder;

    int width, height;

};


#endif //HYPLAYER_CAMERARENDERER_H
