//
// Created by templechen on 2019-09-22.
//

#include <core/SkPath.h>
#include <core/SkFont.h>
#include "SimplePaint3.h"

void SimplePaint3::onDraw(SkCanvas *canvas) {

//    SkPaint paint;
//    paint.setStyle(SkPaint::kStroke_Style);
//    paint.setStrokeWidth(8);
//    paint.setColor(0xff4285F4);
//    paint.setAntiAlias(true);
//    paint.setStrokeCap(SkPaint::kRound_Cap);
//
//    SkPath path;
//    path.moveTo(10, 10);
//    path.quadTo(256, 64, 128, 128);
//    path.quadTo(10, 192, 250, 250);
//    canvas->drawPath(path, paint);

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setTextSize(200);
    paint.setColor(SK_ColorRED);
    paint.setFakeBoldText(true);
    const char* s = "Hello, World";
    canvas->drawText(s, strlen(s), 200, 1000, paint);
}
