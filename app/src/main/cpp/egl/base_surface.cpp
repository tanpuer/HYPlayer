//
// Created by cw on 2018/7/24.
//

#include "base_surface.h"
#include "assert.h"
#include "../base/native_log.h"
#include <GLES2/gl2.h>

base_surface::base_surface(egl_core *eglCore) {
    mEglCore = eglCore;
}

void base_surface::createWindowSurface(ANativeWindow *nativeWindow) {
    assert(mEGLSurface == EGL_NO_SURFACE);
    if (mEGLSurface != EGL_NO_SURFACE){
        ALOGD("createWindowSurface: surface already created");
        return;
    }
    mEGLSurface = mEglCore->createWindowSurface(nativeWindow);
}

void base_surface::createOffscreenSurface(int width, int height) {
    assert(mEGLSurface == EGL_NO_SURFACE);
    if (mEGLSurface != EGL_NO_SURFACE){
        ALOGD("createOffscreenSurface : surface already created");
        return;
    }
    mEGLSurface = mEglCore->createOffscreenSurface(width,height);
    mWidth = width;
    mHeight = height;
}

int base_surface::getHeight() {
    if (mHeight <0){
        return mEglCore->querySurface(mEGLSurface,EGL_HEIGHT);
    }
    return mHeight;
}

int base_surface::getWidth() {
    if (mWidth<0){
        return mEglCore->querySurface(mEGLSurface,EGL_WIDTH);
    }
    return mWidth;
}

void base_surface::releaseEglSurface() {
    mEglCore->releaseSurface(mEGLSurface);
    mEGLSurface = EGL_NO_SURFACE;
    mWidth = mHeight = -1;
}

void base_surface::makeCurrent() {
    mEglCore->makeCurrent(mEGLSurface);
}

void base_surface::swapBuffer() {
    return mEglCore->swapBuffers(mEGLSurface);
}

//获取当前像素
char *base_surface::getCurrentFrame() {
    char* pixels = NULL;
    glReadPixels(0,0,getWidth(),getHeight(),GL_RGBA,GL_UNSIGNED_BYTE,pixels);
    return pixels;
}

void base_surface::makeCurrentReadFrom(base_surface *baseSurface) {
    mEglCore->makeCurrent(this, baseSurface->mEGLSurface);
}

void base_surface::setPresentationTime(long nsecs) {
    mEglCore->setPresentationTime(mEGLSurface, nsecs);
}
