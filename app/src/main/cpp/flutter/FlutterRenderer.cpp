//
// Created by templechen on 2019-09-22.
//

#include <base/native_log.h>
#include <GLES2/gl2.h>
#include "FlutterRenderer.h"

FlutterRenderer::FlutterRenderer() {

}

FlutterRenderer::~FlutterRenderer() {
    if (baseFilter != nullptr) {
        baseFilter->release();
        delete baseFilter;
        baseFilter = nullptr;
    }
    if (windowSurface != nullptr) {
        windowSurface->release(true);
        delete windowSurface;
        windowSurface = nullptr;
    }
    if (eglCore != nullptr) {
//        eglCore->release();
        delete eglCore;
        eglCore = nullptr;
    }
    ALOGD("FlutterRenderer delete");
}

void FlutterRenderer::flutterCreated(ANativeWindow *nativeWindow) {
    ALOGD("templateCreated");
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    windowSurface = new window_surface(nativeWindow, eglCore);

    windowSurface->makeCurrent();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    baseFilter = new FlutterFilter();
}

void FlutterRenderer::flutterChanged(int width, int height) {
    ALOGD("templateChanged");
    windowSurface->makeCurrent();
    glViewport(0, 0, width, height);
    baseFilter->setNativeWindowSize(width, height);
}

void FlutterRenderer::flutterDestroyed() {
    ALOGD("templateDestroyed");
    if (baseFilter != nullptr) {
        baseFilter->release();
        delete baseFilter;
        baseFilter = nullptr;
    }
    if (windowSurface != nullptr) {
        windowSurface->release(true);
        delete windowSurface;
        windowSurface = nullptr;
    }
    if (eglCore != nullptr) {
//        eglCore->release();
        delete eglCore;
        eglCore = nullptr;
    }
}

void FlutterRenderer::flutterDoFrame(long frameTimeNanos) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    baseFilter->doFrame();

    windowSurface->swapBuffer();
}
