//
// Created by templechen on 2019-09-24.
//

#ifndef HYPLAYER_TEXTVIEW_H
#define HYPLAYER_TEXTVIEW_H


#include "View.h"

class TextView : public View {

public:

    char *text;
    int textSize;
    SkColor color;
    bool bold;

    void onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) override;

};


#endif //HYPLAYER_TEXTVIEW_H
