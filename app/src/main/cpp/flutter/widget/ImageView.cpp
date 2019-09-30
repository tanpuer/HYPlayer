//
// Created by templechen on 2019-09-24.
//

#include <core/SkData.h>
#include <core/SkImage.h>
#include <core/SkMaskFilter.h>
#include "ImageView.h"

void ImageView::onDraw(SkCanvas *canvas, int parentWidth, int parentHeight) {
    SkRect destRect;
    destRect.setLTRB(left * parentWidth, top * parentHeight, (width + left) * parentWidth, (top + height) * parentHeight);
//    sk_sp<SkMaskFilter> maskFilter = SkMaskFilter::MakeBlur(SkBlurStyle::kNormal_SkBlurStyle, 100.0);
//    paint.setMaskFilter(maskFilter);
    canvas->drawImageRect(image, *srcRect, destRect, paint);
}

ImageView::ImageView() {
    paint = new SkPaint();
    paint->setAntiAlias(true);
}

ImageView::~ImageView() {
    delete srcRect;
    delete paint;
}

void ImageView::setSrc(char *src) {
    this->src = src;
    sk_sp<SkData> imageData = SkData::MakeFromFileName(src);
    image = SkImage::MakeFromEncoded(imageData);
    srcRect = new SkRect();
    srcRect->setWH(image->width(), image->height());
}
