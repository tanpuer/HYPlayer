//
// Created by templechen on 2019-11-26.
//

#include <base/native_log.h>
#include <GLES3/gl3.h>
#include "ObjViewerRenderer.h"
#include "ObjViewerCubeTextureFilter.h"
#include "NewObjFilter.h"

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
    glEnable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glCullFace(GL_CCW);
    glEnable(GL_BLEND);

//    baseFilter = new ObjViewerFilter();
//    baseFilter = new ObjViewerCubeTextureFilter();
    baseFilter = new NewObjFilter();
    baseFilter->init();
}

void ObjViewerRenderer::objViewerChanged(int width, int height) {
    ALOGD("templateChanged");
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

void ObjViewerRenderer::objViewerOnScroll(int scrollX, int scrollY) {
    if (baseFilter != nullptr) {
        baseFilter->scroll(scrollX, scrollY);
    }
}

void ObjViewerRenderer::objViewerOnScale(float scale) {
    if (baseFilter != nullptr) {
        baseFilter->scale(scale);
    }
}
