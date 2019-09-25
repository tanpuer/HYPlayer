//
// Created by templechen on 2019-09-24.
//

#include <core/SkData.h>
#include <core/SkImage.h>
#include "ImageView.h"

void ImageView::onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) {
    SkPaint paint;
    paint.setAntiAlias(true);
    sk_sp<SkData> imageData = SkData::MakeFromFileName(src);
    auto image = SkImage::MakeFromEncoded(imageData);
    SkRect srcRect;
    srcRect.setWH(image->width(), image->height());
    SkRect destRect;
    destRect.setLTRB(left * parentWidth, top * parentHeight, (width + left) * parentWidth, (top + height) * parentHeight);
    canvas->drawImageRect(image, srcRect, destRect, &paint);
}
