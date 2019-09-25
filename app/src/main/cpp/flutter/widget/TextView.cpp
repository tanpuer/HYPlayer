//
// Created by templechen on 2019-09-24.
//

#include "TextView.h"

void TextView::onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) {
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setTextSize(textSize);
    paint.setColor(color);
    canvas->drawText(text, strlen(text), left * parentWidth, top * parentHeight, paint);
}
