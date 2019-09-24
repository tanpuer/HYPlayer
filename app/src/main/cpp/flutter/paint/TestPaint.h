//
// Created by templechen on 2019-09-22.
//

#ifndef HYPLAYER_TESTPAINT_H
#define HYPLAYER_TESTPAINT_H


#include "FlutterBasePaint.h"

class TestPaint : public FlutterBasePaint{

    virtual void onDraw(SkCanvas *canvas, int windowWidth, int windowHeight);

    virtual ~TestPaint();
};


#endif //HYPLAYER_TESTPAINT_H
