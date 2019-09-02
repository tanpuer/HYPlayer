//
// Created by templechen on 2019-09-02.
//

#include "TemplateBaseFilter.h"
#include "../image/ImageCreator.h"

TemplateBaseFilter::TemplateBaseFilter() {
    ImageCreator *imageCreator = new ImageCreator();
    imageCreator->readImage("/sdcard/test.png");
}

TemplateBaseFilter::~TemplateBaseFilter() {

}
