//
// Created by templechen on 2019-09-06.
//

#ifndef HYPLAYER_NDKCODECENCODER_H
#define HYPLAYER_NDKCODECENCODER_H


#include <jni.h>
#include "../template/TemplateBaseFilter.h"
#include "../egl/window_surface.h"
#include "VideoEncoder.h"

class NDKCodecEncoder {

public:

    NDKCodecEncoder(int width, int height, int bitRate, const char *path, JavaVM *vm);

    ~NDKCodecEncoder();

    void templateCreated();

private:

    egl_core *eglCore;
    window_surface *inputSurface;
    TemplateBaseFilter *baseFilter;
    JNIEnv *env;
    VideoEncoder *encoder;

    int width;
    int height;
    int bitRate;
    const char *path;
    JavaVM *vm;
    pthread_t worker_thread;
    static void *trampoline(void *p);
};


#endif //HYPLAYER_NDKCODECENCODER_H
