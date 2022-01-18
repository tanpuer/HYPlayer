//
// Created by templechen on 2019-09-22.
//

#ifndef HYPLAYER_FLUTTERBASEPAINT_H
#define HYPLAYER_FLUTTERBASEPAINT_H


#include "../../skia/core/SkCanvas.h"

class FlutterBasePaint {

public:

    FlutterBasePaint() {}

    virtual ~FlutterBasePaint() {}

    virtual void onDraw(SkCanvas *canvas, int windowWidth, int windowHeight) = 0;

};


#endif //HYPLAYER_FLUTTERBASEPAINT_H
