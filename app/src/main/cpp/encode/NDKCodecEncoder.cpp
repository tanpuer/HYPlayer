//
// Created by templechen on 2019-09-06.
//

#include <android/native_window_jni.h>
#include <pthread.h>
#include "NDKCodecEncoder.h"
#include "../base/utils.h"
#include "../base/native_log.h"

NDKCodecEncoder::NDKCodecEncoder(int width, int height, int bitRate, const char *path, JavaVM *vm) {
    this->width = width;
    this->height = height;
    this->bitRate = bitRate;
    this->path = path;
    this->vm = vm;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&worker_thread, &attr, trampoline, this);
}

NDKCodecEncoder::~NDKCodecEncoder() {
    delete encoder;
}

void NDKCodecEncoder::templateCreated() {
    long start = javaTimeMillis();

    baseFilter = new TemplateBaseFilter();
    baseFilter->setNativeWindowSize(width, height);

    vm->AttachCurrentThread(&env, nullptr);
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    encoder = new VideoEncoder();
    ANativeWindow *nativeWindow = encoder->nativeWindow;
    ALOGD("ANativeWindow create success");
    inputSurface = new window_surface(nativeWindow, eglCore);
    inputSurface->makeCurrent();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, width, height);

    for (int i = 0; i < 480; i++) {
//        ALOGD("offscreen draw time %d", i);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        //recording start
        inputSurface->makeCurrent();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        baseFilter->doFrame();

        inputSurface->setPresentationTime(i * 16667000LL);
        inputSurface->swapBuffer();
        ALOGD("ndk drainEncoderWithNoTimeOut false");
        encoder->drainEncoderWithNoTimeOut(false);
        //recording end
    }
    ALOGD("ndk drainEncoderWithNoTimeOut true");
    encoder->drainEncoderWithNoTimeOut(true);
    encoder->release();
    ALOGD("offscreen draw over, time is  %ld", javaTimeMillis() - start);

    if (baseFilter != nullptr) {
        baseFilter->release();
        delete baseFilter;
        baseFilter = nullptr;
    }
    if (inputSurface != nullptr) {
        inputSurface->release(false);
        delete inputSurface;
        inputSurface = nullptr;
    }
    if (eglCore != nullptr) {
        delete eglCore;
        eglCore = nullptr;
    }
    vm->DetachCurrentThread();
}

void *NDKCodecEncoder::trampoline(void *p) {
    ((NDKCodecEncoder*) p)->templateCreated();
    return nullptr;
}
