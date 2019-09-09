//
// Created by templechen on 2019-09-05.
//

#include "OffscreenRenderer.h"
#include "android/native_window_jni.h"
#include "../base/native_log.h"
#include "../base/utils.h"

OffscreenRenderer::OffscreenRenderer() {

}

OffscreenRenderer::~OffscreenRenderer() {

}

void
OffscreenRenderer::templateCreated(int width, int height, jobject surface, jobject javaMediaEncoder,
                                   JavaVM *vm) {
    long start = javaTimeMillis();

    vm->AttachCurrentThread(&env, nullptr);
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    ALOGD("ANativeWindow create success");
    inputSurface = new window_surface(nativeWindow, eglCore);

    inputSurface->makeCurrent();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, width, height);

    baseFilter = new TemplateBaseFilter();
    baseFilter->setNativeWindowSize(width, height);

    jclass clazz = env->GetObjectClass(javaMediaEncoder);
    jmethodID jmethodId = env->GetMethodID(clazz, "drainEncoderWithNoTimeOut", "(Z)V");
    for (int i = 0; i < 480; i++) {
        ALOGD("offscreen draw time %d", i);
        //recording start
        inputSurface->makeCurrent();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        baseFilter->doFrame();

        inputSurface->setPresentationTime(i * 16667000LL);
        inputSurface->swapBuffer();
        env->CallVoidMethod(javaMediaEncoder, jmethodId, false);
        //recording end
    }
    env->CallVoidMethod(javaMediaEncoder, jmethodId, true);
    ALOGD("offscreen draw over, time is  %ld", javaTimeMillis() - start);

    if (baseFilter != nullptr) {
        baseFilter->release();
        delete baseFilter;
        baseFilter = nullptr;
    }
    if (inputSurface != nullptr) {
        inputSurface->release(true);
        delete inputSurface;
        inputSurface = nullptr;
    }
    if (eglCore != nullptr) {
        delete eglCore;
        eglCore = nullptr;
    }

    env->DeleteGlobalRef(surface);
    env->DeleteGlobalRef(javaMediaEncoder);
    vm->DetachCurrentThread();
}
