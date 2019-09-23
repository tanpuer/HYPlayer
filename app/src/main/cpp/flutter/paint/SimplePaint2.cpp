//
// Created by templechen on 2019-09-22.
//

#include <skia/effects/SkDiscretePathEffect.h>
#include <skia/effects/SkGradientShader.h>
#include <skia/core/SkShader.h>
#include "SimplePaint2.h"

void SimplePaint2::onDraw(SkCanvas *canvas) {
    canvas->drawColor(SK_ColorWHITE, SkBlendMode::kSrcOut);
    SkPaint paint;
//    paint.setPathEffect(SkDiscretePathEffect::Make(1.0f, 4.0f));
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(2.0f);
    paint.setAntiAlias(true);
    paint.setColor(0xff4285F4);
    SkScalar R = 500.0f, C = 600.0f;
    SkPath path;
    path.moveTo(C + R, C);
    for (int i = 1; i < 8; ++i) {
        SkScalar a = 2.6927937f * i;
        path.lineTo(C + R * cos(a), C + R * sin(a));
    }
    canvas->drawPath(path, paint);
}
