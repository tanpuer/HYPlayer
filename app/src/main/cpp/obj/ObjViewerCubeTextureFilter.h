//
// Created by templechen on 2019-11-27.
//

#ifndef HYPLAYER_OBJVIEWERCUBETEXTUREFILTER_H
#define HYPLAYER_OBJVIEWERCUBETEXTUREFILTER_H


#include "ObjViewerFilter.h"
#include "string"
#include "vector"

class ObjViewerCubeTextureFilter : public ObjViewerFilter {

public:

    ObjViewerCubeTextureFilter();

    void init() override;

    void release() override;

protected:

    void initShaders() override;

    std::vector<const char *> files = {"/sdcard/right.tga", "/sdcard/left.tga",
                                       "/sdcard/bottom.tga", "/sdcard/top.tga", "/sdcard/front.tga",
                                       "/sdcard/back.tga"};

    GLuint texId = GL_INVALID_VALUE;

};


#endif //HYPLAYER_OBJVIEWERCUBETEXTUREFILTER_H
