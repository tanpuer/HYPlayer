//
// Created by templechen on 2019-09-22.
//

#ifndef HYPLAYER_FLUTTERFILTER_H
#define HYPLAYER_FLUTTERFILTER_H


#include <GLES2/gl2.h>
#include <base/matrix_util.h>
#include <image/IAVFrameCreator.h>
#include <gpu/GrBackendSurface.h>
#include <core/SkSurface.h>
#include <flutter/paint/FlutterBasePaint.h>

class FlutterFilter {

public:

    FlutterFilter();

    virtual ~FlutterFilter();

    virtual void setNativeWindowSize(int width, int height);

    virtual void doFrame();

    virtual void release();

    int time = 0;

protected:
    sk_sp<GrDirectContext> context = nullptr;
    sk_sp<SkSurface> skia_surface = nullptr;

    int windowWidth;
    int windowHeight;

    FlutterBasePaint *basePaint;
};


#endif //HYPLAYER_FLUTTERFILTER_H
