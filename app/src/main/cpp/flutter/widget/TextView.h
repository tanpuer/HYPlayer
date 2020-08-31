//
// Created by templechen on 2019-09-24.
//

#ifndef HYPLAYER_TEXTVIEW_H
#define HYPLAYER_TEXTVIEW_H


#include "View.h"
#include "vector"

class TextView : public View {

public:

    char *text;
    int textSize;
    SkColor color;
    bool bold;

    void onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) override;

    TextView();

    virtual ~TextView();

    void setText(char *text);

    void setTextSize(int textSize);

    void setColor(SkColor color);

    void setBold(bool bold);

private:

    SkPaint *paint;

    std::vector<float> startList;
};


#endif //HYPLAYER_TEXTVIEW_H
