//
// Created by templechen on 2019-09-02.
//

#include "TemplateRenderer.h"
#include "../base/native_log.h"
#include <GLES3/gl3.h>

TemplateRenderer::TemplateRenderer() {

}

TemplateRenderer::~TemplateRenderer() {
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
    ALOGD("TemplateRenderer delete");
}

void TemplateRenderer::templateCreated(ANativeWindow *nativeWindow) {
    ALOGD("templateCreated");
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    windowSurface = new window_surface(nativeWindow, eglCore);

    windowSurface->makeCurrent();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    baseFilter = new TemplateBaseFilter();
}

void TemplateRenderer::templateChanged(int width, int height) {
    ALOGD("templateChanged");
    windowSurface->makeCurrent();
    glViewport(0, 0, width, height);
    baseFilter->setNativeWindowSize(width, height);
}

void TemplateRenderer::templateDestroyed() {
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

void TemplateRenderer::templateDoFrame(long frameTimeNanos) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    baseFilter->doFrame();

    windowSurface->swapBuffer();
}
