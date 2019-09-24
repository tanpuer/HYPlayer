//
// Created by templechen on 2019-09-22.
//

#include <core/SkPath.h>
#include <core/SkFont.h>
#include <core/SkImage.h>
#include <base/native_log.h>
#include "TestPaint.h"

void TestPaint::onDraw(SkCanvas *canvas) {

    SkPaint imagePaint;
    imagePaint.setAntiAlias(true);
    sk_sp<SkData> imageData = SkData::MakeFromFileName("sdcard/test.jpeg");
    auto image = SkImage::MakeFromEncoded(imageData);
    ALOGD("%d %d", image->width(), image->height());
//    SkRect rect = SkRect::MakeXYWH(0, 0, 1080, 1920);
//    canvas->drawImageRect(image, rect, rect, &imagePaint);
    canvas->drawImage(image, 500, 500, &imagePaint);

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setTextSize(200);
    paint.setColor(SK_ColorRED);
    paint.setFakeBoldText(true);
    const char *s = "Hello, World";
    canvas->drawText(s, strlen(s), 200, 1000, paint);


    SkPaint pathPaint;
    pathPaint.setStyle(SkPaint::kFill_Style);
    pathPaint.setAntiAlias(true);
    pathPaint.setStrokeWidth(4);
    pathPaint.setColor(0xff4285F4);
    SkRect rect = SkRect::MakeXYWH(100, 100, 300, 420);
    canvas->drawRect(rect, pathPaint);
    SkRRect oval;
    oval.setOval(rect);
    oval.offset(40, 80);
    pathPaint.setColor(0xffDB4437);
    canvas->drawRRect(oval, pathPaint);
    pathPaint.setColor(0xff0F9D58);
    canvas->drawCircle(180, 50, 25, pathPaint);
    rect.offset(80, 50);
    pathPaint.setColor(0xffF4B400);
    pathPaint.setStyle(SkPaint::kStroke_Style);
    canvas->drawRoundRect(rect, 10, 10, pathPaint);


    {
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

}

TestPaint::~TestPaint() {

}

