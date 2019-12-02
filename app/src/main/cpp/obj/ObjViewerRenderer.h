//
// Created by templechen on 2019-11-26.
//

#ifndef HYPLAYER_OBJVIEWERRENDERER_H
#define HYPLAYER_OBJVIEWERRENDERER_H


#include <android/native_window.h>
#include <egl/window_surface.h>
#include "ObjViewerFilter.h"

class ObjViewerRenderer {

public:

    ObjViewerRenderer();

    virtual ~ObjViewerRenderer();

    void objViewerCreated(ANativeWindow *nativeWindow);

    void objViewerChanged(int width, int height);

    void objViewerDestroyed();

    void objViewerDoFrame();

    void objViewerOnScroll(int scrollX, int scrollY);

private:

    egl_core *eglCore;

    window_surface *windowSurface;

    ObjViewerFilter *baseFilter;

};


#endif //HYPLAYER_OBJVIEWERRENDERER_H
