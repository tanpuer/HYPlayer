//
// Created by templechen on 2019-09-22.
//

#include <base/gl_utils.h>
#include "FlutterFilter.h"
#include <skia/gpu/gl/GrGLInterface.h>
#include <skia/core/SkPaint.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkGraphics.h>
#include <skia/gpu/GrBackendSurface.h>
#include <skia/gpu/gl/GrGLDefines.h>
#include <template/TemplateBaseFilter.h>
#include <flutter/paint/TestPaint.h>
#include <base/utils.h>

#define GET_STR(x) #x

FlutterFilter::FlutterFilter() {
    basePaint = new TestPaint();
    SkGraphics::Init();
}

FlutterFilter::~FlutterFilter() {

}

void FlutterFilter::setNativeWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;

    if (skia_surface == nullptr || skia_surface->width() != width ||
        skia_surface->height() != height) {
        sk_sp<const GrGLInterface> interface(GrGLMakeNativeInterface());
        context = GrContext::MakeGL(interface);
        SkASSERT(context);
        // Wrap the frame buffer object attached to the screen in a Skia render target so Skia can
        // render to it
        GrGLFramebufferInfo info;
//        info.fFBOID = frameBuffer;
        info.fFBOID = 0;
        SkColorType colorType;
        info.fFormat = GR_GL_RGBA8;
        colorType = kRGBA_8888_SkColorType;
        GrBackendRenderTarget target(windowWidth, windowHeight, 0, 8, info);
        // setup SkSurface
        // To use distance field text, use commented out SkSurfaceProps instead
        // SkSurfaceProps props(SkSurfaceProps::kUseDeviceIndependentFonts_Flag,
        //                      SkSurfaceProps::kLegacyFontHost_InitType);
        SkSurfaceProps props(SkSurfaceProps::kLegacyFontHost_InitType);
        skia_surface = (SkSurface::MakeFromBackendRenderTarget(context.get(), target,
                                                               kBottomLeft_GrSurfaceOrigin,
                                                               colorType, nullptr, &props));
        SkASSERT(skia_surface);
    }
}

void FlutterFilter::doFrame() {
    long start = javaTimeMillis();
    SkCanvas *canvas = skia_surface->getCanvas();
    basePaint->onDraw(canvas, windowWidth, windowHeight);
    canvas->flush();
    ALOGD("Flutter draw time %ld", javaTimeMillis() - start);
}

void FlutterFilter::release() {
    ALOGD("FlutterFilter release");
    delete basePaint;
    basePaint = nullptr;
}