//
// Created by templechen on 2019-11-26.
//

#include <base/native_log.h>
#include <GLES3/gl3.h>
#include "ObjViewerRenderer.h"

ObjViewerRenderer::ObjViewerRenderer() {

}

ObjViewerRenderer::~ObjViewerRenderer() {
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

void ObjViewerRenderer::objViewerCreated(ANativeWindow *nativeWindow) {
    ALOGD("templateCreated");
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    windowSurface = new window_surface(nativeWindow, eglCore);

    windowSurface->makeCurrent();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    baseFilter = new ObjViewerFilter();
}

void ObjViewerRenderer::objViewerChanged(int width, int height) {
    ALOGD("templateChanged");
    windowSurface->makeCurrent();
    glViewport(0, 0, width, height);
    baseFilter->setNativeWindowSize(width, height);
}

void ObjViewerRenderer::objViewerDestroyed() {
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

void ObjViewerRenderer::objViewerDoFrame() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    baseFilter->doFrame();

    windowSurface->swapBuffer();
}
