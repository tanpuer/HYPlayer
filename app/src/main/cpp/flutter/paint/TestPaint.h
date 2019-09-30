//
// Created by templechen on 2019-09-22.
//

#ifndef HYPLAYER_TESTPAINT_H
#define HYPLAYER_TESTPAINT_H


#include "FlutterBasePaint.h"
#include "widget/View.h"
#include "widget/TextView.h"
#include "widget/ImageView.h"

class TestPaint : public FlutterBasePaint {

public:

    TestPaint();

    virtual void onDraw(SkCanvas *canvas, int windowWidth, int windowHeight);

    virtual ~TestPaint();

private:

    View *view;
    TextView *textView;
    ImageView *imageView;
};


#endif //HYPLAYER_TESTPAINT_H
