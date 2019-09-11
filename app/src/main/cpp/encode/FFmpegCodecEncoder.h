//
// Created by templechen on 2019-09-11.
//

#ifndef HYPLAYER_FFMPEGCODECENCODER_H
#define HYPLAYER_FFMPEGCODECENCODER_H


#include <jni.h>
#include "../template/TemplateBaseFilter.h"
#include "../egl/offscreen_surface.h"
#include "FFVideoEncoder.h"

class FFmpegCodecEncoder {

public:

    FFmpegCodecEncoder(int width, int height, int bitRate, const char *path, JavaVM *vm);

    ~FFmpegCodecEncoder();

    void templateCreated();

private:

    egl_core *eglCore;
    offscreen_surface *inputSurface;
    TemplateBaseFilter *baseFilter;
    JNIEnv *env;

    FFVideoEncoder *encoder;

    int width;
    int height;
    int bitRate;
    const char *path;
    JavaVM *vm;
    pthread_t worker_thread;
    static void *trampoline(void *p);

};


#endif //HYPLAYER_FFMPEGCODECENCODER_H
