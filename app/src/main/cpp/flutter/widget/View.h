//
// Created by templechen on 2019-09-24.
//

#ifndef HYPLAYER_VIEW_H
#define HYPLAYER_VIEW_H


#include <core/SkColor.h>
#include <core/SkCanvas.h>

class View {

public:

    //只支持百分比布局
    float top;
    float left;
    float width;
    float height;
    int cornerRadius;
    SkColor backgroundColor;

    virtual void onDraw(SkCanvas *canvas, int parentWidth, int parentHeight);

};


#endif //HYPLAYER_VIEW_H
