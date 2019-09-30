//
// Created by templechen on 2019-09-24.
//

#include "TextView.h"

void TextView::onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) {
    canvas->drawText(text, strlen(text), left * parentWidth, top * parentHeight, *paint);
}

TextView::TextView() {
    paint = new SkPaint();
    paint->setAntiAlias(true);
}

TextView::~TextView() {
    delete paint;
}

void TextView::setText(char *text) {
    TextView::text = text;
}

void TextView::setTextSize(int textSize) {
    TextView::textSize = textSize;
    paint->setTextSize(textSize);
}

void TextView::setColor(SkColor color) {
    TextView::color = color;
    paint->setColor(color);
}

void TextView::setBold(bool bold) {
    TextView::bold = bold;
    paint->setFakeBoldText(bold);
}
