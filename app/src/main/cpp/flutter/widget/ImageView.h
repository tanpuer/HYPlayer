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

    ImageView();

    virtual ~ImageView();

    void setSrc(char *src);

private:

    SkPaint *paint;

    sk_sp<SkImage> image;

    SkRect *srcRect;
};


#endif //HYPLAYER_IMAGEVIEW_H
