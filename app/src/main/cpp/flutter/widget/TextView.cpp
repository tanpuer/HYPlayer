//
// Created by templechen on 2019-09-24.
//

#include <base/native_log.h>
#include "TextView.h"
#include "SkTypeface.h"
#include "utils.h"
#include "random"

using std::default_random_engine;
using std::uniform_real_distribution;

void TextView::onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) {
    long start = javaTimeMillis();

    for (int i = 0; i < startList.size(); i += 1) {
        if (startList[i] < -0.3f) {
            startList[i] = 1.0f;
        }
        canvas->drawText(text, strlen(text), startList[i] * parentWidth, i * parentHeight / 100,
                         *paint);
        startList[i] -= 0.005f;
    }
//    ALOGD("SK TextView: %ld", javaTimeMillis() - start)
}

TextView::TextView() {
    paint = new SkPaint();
    paint->setAntiAlias(true);
    paint->setTypeface(SkTypeface::MakeFromFile("/sdcard/Kaiw5-gb5-2.ttf"));
    startList = std::vector<float>(100, 0.0);
    default_random_engine e;
    for (float &i : startList) {
        uniform_real_distribution<double> u(1.0, 2.0);
        i = u(e);
    }
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
