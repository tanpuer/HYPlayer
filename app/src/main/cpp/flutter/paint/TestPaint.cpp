//
// Created by templechen on 2019-09-22.
//

#include <core/SkPath.h>
#include <core/SkFont.h>
#include <core/SkImage.h>
#include <base/native_log.h>
#include <flutter/widget/View.h>
#include <flutter/widget/TextView.h>
#include <flutter/widget/ImageView.h>
#include "TestPaint.h"

TestPaint::TestPaint() {
    view = new View();
    view->setCornerRadius(20);
    view->setBackgroundColor(SK_ColorGREEN);
    view->setRect(0.1f, 0.1f, 0.5f, 0.1f);

    textView = new TextView();
    textView->setRect(0.5f, 0.3f, 0.3f, 0.1f);
    textView->setColor(SK_ColorRED);
    textView->setBold(true);
    textView->setTextSize(100);
    textView->setText("Hello World!");

    imageView = new ImageView();
    imageView->setRect(0.4f, 0.7f, 0.5f, 0.5f);
    imageView->setSrc("sdcard/test.jpeg");
}

void TestPaint::onDraw(SkCanvas *canvas, int windowWidth, int windowHeight) {

    //View
    view->onDraw(canvas, windowWidth, windowHeight);

    //TextView
    textView->onDraw(canvas, windowWidth, windowHeight);

    //ImageView
    imageView->onDraw(canvas, windowWidth, windowHeight);
}

TestPaint::~TestPaint() {
    delete view;
    delete textView;
    delete imageView;
}
