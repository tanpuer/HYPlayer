//
// Created by templechen on 2019-09-02.
//

#ifndef HYPLAYER_TEMPLATERENDERER_H
#define HYPLAYER_TEMPLATERENDERER_H


#include <android/native_window.h>
#include "../egl/egl_core.h"
#include "../egl/window_surface.h"

class TemplateRenderer {

public:

    TemplateRenderer();

    virtual ~TemplateRenderer();

    void templateCreated(ANativeWindow *nativeWindow);

    void templateChanged(int width, int height);

    void templateDestroyed();

    void templateDoFrame(long frameTimeNanos);

private:

    egl_core *eglCore;

    window_surface *windowSurface;

};


#endif //HYPLAYER_TEMPLATERENDERER_H
