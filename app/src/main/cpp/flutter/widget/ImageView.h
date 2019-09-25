//
// Created by templechen on 2019-09-24.
//

#ifndef HYPLAYER_IMAGEVIEW_H
#define HYPLAYER_IMAGEVIEW_H


#include "View.h"

class ImageView : public View {

public:

    enum {
        FIT_CENTER,
        CENTER_CROP
    };

    char *src;
    int scaleType;

    void onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) override;
};


#endif //HYPLAYER_IMAGEVIEW_H
