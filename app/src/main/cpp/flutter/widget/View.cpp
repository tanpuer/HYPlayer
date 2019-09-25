//
// Created by templechen on 2019-09-24.
//

#include "View.h"

void View::onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) {
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(backgroundColor);
    SkRect rect = SkRect::MakeXYWH(parentWidth * left, parentHeight * top, parentWidth * width,
                                   parentHeight * height);
    canvas->drawRoundRect(rect, cornerRadius, cornerRadius, paint);
}
