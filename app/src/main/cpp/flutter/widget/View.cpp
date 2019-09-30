//
// Created by templechen on 2019-09-24.
//

#include "View.h"

void View::onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) {
    SkRect rect = SkRect::MakeXYWH(parentWidth * left, parentHeight * top, parentWidth * width,
                                   parentHeight * height);
    canvas->drawRoundRect(rect, cornerRadius, cornerRadius, *paint);
}

View::View() {
    paint = new SkPaint();
    paint->setAntiAlias(true);
    paint->setStyle(SkPaint::kFill_Style);
}

View::~View() {
    delete paint;
}

void View::setRect(float top, float left, float width, float height) {
    this->top = top;
    this->left = left;
    this->width = width;
    this->height = height;
}

void View::setBackgroundColor(SkColor backgroundColor) {
    this->backgroundColor = backgroundColor;
    paint->setColor(backgroundColor);
}

void View::setCornerRadius(int cornerRadius) {
    this->cornerRadius = cornerRadius;
}
