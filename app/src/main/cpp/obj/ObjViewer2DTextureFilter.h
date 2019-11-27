//
// Created by templechen on 2019-11-27.
//

#ifndef HYPLAYER_OBJVIEWER2DTEXTUREFILTER_H
#define HYPLAYER_OBJVIEWER2DTEXTUREFILTER_H


#include "ObjViewerFilter.h"
#include "Texture2D.h"

class ObjViewer2DTextureFilter : public ObjViewerFilter{

protected:

    void initShaders() override;

public:

    void init() override;

    void release() override;

private:

    GLuint texVBO;
    Texture2D *texture2D = nullptr;
};


#endif //HYPLAYER_OBJVIEWER2DTEXTUREFILTER_H
