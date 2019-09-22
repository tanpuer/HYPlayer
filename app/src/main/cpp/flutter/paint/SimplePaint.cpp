//
// Created by templechen on 2019-09-22.
//

#include "SimplePaint.h"
#include <skia/gpu/gl/GrGLInterface.h>
#include <skia/core/SkPaint.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkGraphics.h>
#include <skia/gpu/GrBackendSurface.h>
#include <skia/effects/SkDiscretePathEffect.h>
#include <skia/gpu/gl/GrGLDefines.h>
#include <skia/effects/Sk2DPathEffect.h>

void SimplePaint::onDraw(SkCanvas *canvas) {
    canvas->drawColor(SK_ColorWHITE, SkBlendMode::kSrcOut);
    SkPaint paint;
    paint.setStyle(SkPaint::kFill_Style);
    paint.setAntiAlias(true);
    paint.setStrokeWidth(4);
    paint.setColor(0xff4285F4);
    SkRect rect = SkRect::MakeXYWH(100, 100, 300, 420);
    canvas->drawRect(rect, paint);
    SkRRect oval;
    oval.setOval(rect);
    oval.offset(40, 80);
    paint.setColor(0xffDB4437);
    canvas->drawRRect(oval, paint);
    paint.setColor(0xff0F9D58);
    canvas->drawCircle(180, 50, 25, paint);
    rect.offset(80, 50);
    paint.setColor(0xffF4B400);
    paint.setStyle(SkPaint::kStroke_Style);
    canvas->drawRoundRect(rect, 10, 10, paint);
}
