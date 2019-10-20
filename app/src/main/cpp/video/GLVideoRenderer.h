//
// Created by templechen on 2019-04-28.
//

#ifndef FFMPEG_PLAYER_GL_RENDERER_H
#define FFMPEG_PLAYER_GL_RENDERER_H

#include <android/native_window.h>
#include <decode/circle_av_frame_queue.h>
#include "../egl/egl_core.h"
#include "../egl/window_surface.h"
#include "base_filter.h"

extern "C" {
#include <libavutil/frame.h>
};

class GLVideoRenderer {

public:
    GLVideoRenderer(circle_av_frame_queue *frameQueue);

    virtual ~GLVideoRenderer();

    void surfaceCreated(ANativeWindow *nativeWindow);

    void surfaceChanged(int width, int height);

    void surfaceDestroyed();

    void surfaceDoFrame();

    void start();

    void pause();

private:

    circle_av_frame_queue *frameQueue;

    egl_core *eglCore;

    window_surface *windowSurface = nullptr;

    base_filter *filter = nullptr;

    void initFilter(AVFrame *avFrame);

    int screen_width, screen_height;

    bool started = false;
};

#endif //FFMPEG_PLAYER_GL_RENDERER_H
