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

void TestPaint::onDraw(SkCanvas *canvas, int windowWidth, int windowHeight) {

//    SkPaint imagePaint;
//    imagePaint.setAntiAlias(true);
//    sk_sp<SkData> imageData = SkData::MakeFromFileName("sdcard/test.jpeg");
//    auto image = SkImage::MakeFromEncoded(imageData);
//    SkRect src1;
//    src1.setWH(image->width(), image->height());
//    SkRect dst1;
//    dst1.setLTRB(0, 0, windowWidth, windowHeight);
//    canvas->drawImageRect(image, src1, dst1, &imagePaint);
////    canvas->drawImage(image, 500, 500, &imagePaint);
//
//    SkPaint paint;
//    paint.setAntiAlias(true);
//    paint.setTextSize(200);
//    paint.setColor(SK_ColorRED);
//    paint.setFakeBoldText(true);
//    const char *s = "Hello, World";
//    canvas->drawText(s, strlen(s), 200, 1000, paint);
//
//
//    SkPaint pathPaint;
//    pathPaint.setStyle(SkPaint::kFill_Style);
//    pathPaint.setAntiAlias(true);
//    pathPaint.setStrokeWidth(4);
//    pathPaint.setColor(0xff4285F4);
//    SkRect rect = SkRect::MakeXYWH(100, 100, 300, 420);
//    canvas->drawRect(rect, pathPaint);
//    SkRRect oval;
//    oval.setOval(rect);
//    oval.offset(40, 80);
//    pathPaint.setColor(0xffDB4437);
//    canvas->drawRRect(oval, pathPaint);
//    pathPaint.setColor(0xff0F9D58);
//    canvas->drawCircle(180, 50, 25, pathPaint);
//    rect.offset(80, 50);
//    pathPaint.setColor(0xffF4B400);
//    pathPaint.setStyle(SkPaint::kStroke_Style);
//    canvas->drawRoundRect(rect, 10, 10, pathPaint);
//
//
//    {
//        SkPaint paint;
////    paint.setPathEffect(SkDiscretePathEffect::Make(1.0f, 4.0f));
//        paint.setStyle(SkPaint::kStroke_Style);
//        paint.setStrokeWidth(2.0f);
//        paint.setAntiAlias(true);
//        paint.setColor(0xff4285F4);
//        SkScalar R = 500.0f, C = 600.0f;
//        SkPath path;
//        path.moveTo(C + R, C);
//        for (int i = 1; i < 8; ++i) {
//            SkScalar a = 2.6927937f * i;
//            path.lineTo(C + R * cos(a), C + R * sin(a));
//        }
//        canvas->drawPath(path, paint);
//    }


    //View
    View view;
    view.left = 0.1f;
    view.top = 0.1f;
    view.width = 0.5f;
    view.height = 0.1f;
    view.backgroundColor = SK_ColorGREEN;
    view.cornerRadius = 20;
    view.onDraw(canvas, windowWidth, windowHeight);

    //TextView
    TextView textView;
    textView.left = 0.1f;
    textView.top = 0.3f;
    textView.width = 0.3f;
    textView.height = 0.1f;
    textView.color = SK_ColorRED;
    textView.bold = true;
    textView.text = "Hello World!";
    textView.textSize = 100;
    textView.onDraw(canvas, windowWidth, windowHeight);

    //ImageView
    ImageView imageView;
    imageView.left = 0.7f;
    imageView.top = 0.4f;
    imageView.width = 0.5f;
    imageView.height = 0.5f;
    imageView.src = "sdcard/test.jpeg";
    imageView.onDraw(canvas, windowWidth, windowHeight);

}

TestPaint::~TestPaint() {

}

