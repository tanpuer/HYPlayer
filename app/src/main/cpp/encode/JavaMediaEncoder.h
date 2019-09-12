//
// Created by templechen on 2019-09-05.
//

#ifndef HYPLAYER_JAVAMEDIAENCODER_H
#define HYPLAYER_JAVAMEDIAENCODER_H


#include <jni.h>
#include "../egl/egl_core.h"
#include "../egl/offscreen_surface.h"
#include "../template/TemplateBaseFilter.h"
#include "../egl/window_surface.h"

class JavaMediaEncoder {

public:

    JavaMediaEncoder();

    ~JavaMediaEncoder();

    void templateCreated(int width, int height, jobject surface, jobject javaMediaEncoder, JavaVM *vm);

private:

    egl_core *eglCore;
    window_surface *inputSurface;
    TemplateBaseFilter *baseFilter;
    JNIEnv *env;

};


#endif //HYPLAYER_JAVAMEDIAENCODER_H
