//
// Created by templechen on 2019-09-05.
//

#include "OffscreenRenderer.h"
#include "android/native_window_jni.h"
#include "../base/native_log.h"

OffscreenRenderer::OffscreenRenderer() {

}

OffscreenRenderer::~OffscreenRenderer() {

}

void OffscreenRenderer::templateCreated(int width, int height, jobject surface, jobject javaMediaEncoder, JavaVM *vm) {
    vm->AttachCurrentThread(&env, nullptr);

    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    offscreenSurface = new offscreen_surface(eglCore, width, height);

    offscreenSurface->makeCurrent();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, width, height);

    baseFilter = new TemplateBaseFilter();
    baseFilter->setNativeWindowSize(width, height);

    ALOGD("1111 %d", surface == nullptr);
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    ALOGD("2222");

    inputSurface = new window_surface(nativeWindow, eglCore);

    jclass clazz = env->GetObjectClass(javaMediaEncoder);
    jmethodID jmethodId = env->GetMethodID(clazz, "drainEncoderWithNoTimeOut", "(Z)V");

    for (int i = 0; i < 240; i++) {
        ALOGD("offscreen draw time %d", i);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        baseFilter->doFrame();
        offscreenSurface->swapBuffer();

        //recording start
        inputSurface->makeCurrentReadFrom(offscreenSurface);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        baseFilter->doFrame();

        inputSurface->setPresentationTime(i * 16667L * 1000);
        inputSurface->swapBuffer();
        env->CallVoidMethod(javaMediaEncoder, jmethodId, false);
        //recording end
        offscreenSurface->makeCurrent();
        offscreenSurface->swapBuffer();
    }
    env->CallVoidMethod(javaMediaEncoder, jmethodId, true);
    ALOGD("offscreen draw over");
    //
}

void OffscreenRenderer::templateDestroyed() {

}

void OffscreenRenderer::templateDoFrame(long frameTimeNanos) {

}
