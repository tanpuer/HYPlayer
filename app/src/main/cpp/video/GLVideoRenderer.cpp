//
// Created by templechen on 2019-04-28.
//

#include "GLVideoRenderer.h"
#include "mediacodec_nv12_filter.h"
#include "mediacodec_nv21_filter.h"
#include <GLES3/gl3.h>
#include <base/native_log.h>

GLVideoRenderer::GLVideoRenderer(circle_av_frame_queue *frameQueue) {
    this->frameQueue = frameQueue;
    eglCore = nullptr;
    windowSurface = nullptr;
}

GLVideoRenderer::~GLVideoRenderer() {
    if (filter != nullptr) {
        delete filter;
        filter = nullptr;
    }
    if (windowSurface != nullptr) {
        windowSurface->release(true);
        delete windowSurface;
        windowSurface = nullptr;
    }
    if (eglCore != nullptr) {
        eglCore->release();
        delete eglCore;
        eglCore = nullptr;
    }
}

void GLVideoRenderer::surfaceCreated(ANativeWindow *nativeWindow) {
    ALOGD("VideoRenderer created");
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    windowSurface = new window_surface(nativeWindow, eglCore, false);
    windowSurface->makeCurrent();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    filter = new base_filter();
//    filter->init_program();
}

void GLVideoRenderer::surfaceChanged(int width, int height) {
    ALOGD("VideoRenderer changed");
    windowSurface->makeCurrent();
    glViewport(0, 0, width, height);
    windowSurface->swapBuffer();

    screen_width = width;
    screen_height = height;
}

void GLVideoRenderer::surfaceDestroyed() {
//    if (filter != nullptr) {
//        delete filter;
//        filter = nullptr;
//    }
//    if (windowSurface != nullptr) {
//        windowSurface->release();
//        delete windowSurface;
//        windowSurface = nullptr;
//    }
//    if (eglCore != nullptr) {
//        eglCore->release();
//        delete eglCore;
//        eglCore = nullptr;
//    }
}

void GLVideoRenderer::surfaceDoFrame() {


    AVFrameData *data = frameQueue->pull();
    if (data != nullptr && data->frame != nullptr) {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        AVFrame *avFrame = data->frame;
        initFilter(avFrame);
        filter->drawFrame(avFrame);
        windowSurface->swapBuffer();
    }
}

void GLVideoRenderer::initFilter(AVFrame *avFrame) {
    if (filter != nullptr) {
        return;
    }
    switch (avFrame->format) {
        case AV_PIX_FMT_YUV420P: {
            ALOGD("video type is YUV420P!");
            filter = new base_filter();
            break;
        }
        case AV_PIX_FMT_NV12: {
            ALOGD("video type is NV12!");
            filter = new mediacodec_nv12_filter();
            break;
        }
        case AV_PIX_FMT_NV21: {
            ALOGD("video type is NV21!");
            filter = new mediacodec_nv21_filter();
            break;
        }
        default: {
            ALOGE("not support this video type!");
            break;
        }
    }
    if (filter != nullptr) {
        filter->screen_width = screen_width;
        filter->screen_height = screen_height;
        filter->init_program();
    }
}
