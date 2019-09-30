//
// Created by templechen on 2019-09-11.
//

#include <pthread.h>
#include <template/TemplateFBOFilter.h>
#include "FFmpegCodecEncoder.h"
#include "../base/utils.h"
#include "../base/native_log.h"

FFmpegCodecEncoder::FFmpegCodecEncoder(int width, int height, int bitRate, const char *path,
                                       JavaVM *vm) {
    this->width = width;
    this->height = height;
    this->bitRate = bitRate;
    this->path = path;
    this->vm = vm;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&worker_thread, &attr, trampoline, this);
}

FFmpegCodecEncoder::~FFmpegCodecEncoder() {

}

void FFmpegCodecEncoder::templateCreated() {
    long start = javaTimeMillis();

    vm->AttachCurrentThread(&env, nullptr);
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    inputSurface = new offscreen_surface(eglCore, width, height);
    inputSurface->makeCurrent();
    encoder = new FFVideoEncoder();
    encoder->InitEncoder(path, width, height);
    encoder->EncodeStart();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    baseFilter = new TemplateBaseFilter();
    baseFilter = new TemplateFBOFilter();
    baseFilter->setNativeWindowSize(width, height);
    baseFilter->identifyTextureMatrix();

    glViewport(0, 0, width, height);

    for (int i = 0; i < 480; i++) {
//        ALOGD("offscreen draw time %d", i);
        //recording start
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        baseFilter->doFrame();
        //do not need to swapBuffer
//        inputSurface->swapBuffer();
        unsigned char *buffer = static_cast<unsigned char *>(malloc((size_t) width * height * 4));
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        encoder->EncoderBuffer(buffer, i * 16667000LL);
        //recording end
    }
    encoder->EncoderEnd();
    ALOGD("offscreen draw over, time is  %ld", javaTimeMillis() - start);

    if (baseFilter != nullptr) {
        baseFilter->release();
        delete baseFilter;
        baseFilter = nullptr;
    }
    if (inputSurface != nullptr) {
        inputSurface->release();
        delete inputSurface;
        inputSurface = nullptr;
    }
    if (eglCore != nullptr) {
        delete eglCore;
        eglCore = nullptr;
    }
    vm->DetachCurrentThread();
}

void *FFmpegCodecEncoder::trampoline(void *p) {
    ((FFmpegCodecEncoder *) p)->templateCreated();
    return nullptr;
}
