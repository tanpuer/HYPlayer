//
// Created by templechen on 2019-09-22.
//

#ifndef HYPLAYER_FLUTTERRENDERER_H
#define HYPLAYER_FLUTTERRENDERER_H


#include <android/native_window.h>
#include <egl/egl_core.h>
#include <egl/window_surface.h>
#include "FlutterFilter.h"

class FlutterRenderer {

public:

    FlutterRenderer();

    virtual ~FlutterRenderer();

    void flutterCreated(ANativeWindow *nativeWindow);

    void flutterChanged(int width, int height);

    void flutterDestroyed();

    void flutterDoFrame(long frameTimeNanos);

private:

    egl_core *eglCore;

    window_surface *windowSurface;

    FlutterFilter *baseFilter;
};


#endif //HYPLAYER_FLUTTERRENDERER_H
