//
// Created by templechen on 2019-10-11.
//

#include <base/native_log.h>
#include <GLES2/gl2.h>
#include "CameraRenderer.h"

CameraRenderer::CameraRenderer(JavaVM *vm, jobject javaCameraView) {
    this->vm = vm;
    this->javaCameraView = javaCameraView;
    vm->AttachCurrentThread(&env, nullptr);
}

CameraRenderer::~CameraRenderer() {
    vm->DetachCurrentThread();
}

void CameraRenderer::cameraViewCreated(ANativeWindow *nativeWindow) {
    ALOGD("cameraViewCreated");
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    windowSurface = new window_surface(nativeWindow, eglCore);
    windowSurface->makeCurrent();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    baseFilter = new CameraBaseFilter();

    clazz = env->GetObjectClass(this->javaCameraView);
    jmethodID methodId = env->GetMethodID(clazz, "createOESSurface", "(I)V");
    env->CallVoidMethod(this->javaCameraView, methodId, baseFilter->oesTextureId);
}

void CameraRenderer::cameraViewChanged(int width, int height) {
    ALOGD("cameraViewChanged");
    windowSurface->makeCurrent();
    glViewport(0, 0, width, height);
    baseFilter->setNativeWindowSize(width, height);
}

void CameraRenderer::cameraViewDestroyed() {
    ALOGD("cameraViewDestroyed");
    if (javaCameraView != nullptr) {
        env->DeleteGlobalRef(javaCameraView);
    }
    if (baseFilter != nullptr) {
        ALOGD("cameraViewDestroyed success11111");
        baseFilter->release();
        delete baseFilter;
        baseFilter = nullptr;
    }
    ALOGD("cameraViewDestroyed success1");
    if (windowSurface != nullptr) {
        windowSurface->release(true);
        delete windowSurface;
        windowSurface = nullptr;
    }
    ALOGD("cameraViewDestroyed success2");
    if (eglCore != nullptr) {
        eglCore->release();
        delete eglCore;
        eglCore = nullptr;
    }
    ALOGD("cameraViewDestroyed success");
}

void CameraRenderer::cameraViewDoFrame() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

//    jclass clazz = env->GetObjectClass(this->javaCameraView);
    jmethodID methodId = env->GetMethodID(clazz, "update", "()V");
    env->CallVoidMethod(this->javaCameraView, methodId);

    baseFilter->doFrame();

    windowSurface->swapBuffer();
}

void CameraRenderer::cameraSizeChanged(int width, int height) {
    if (baseFilter != nullptr) {
        baseFilter->setVideoSize(width, height);
    }
}
