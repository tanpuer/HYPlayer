//
// Created by templechen on 2019-09-02.
//

#ifndef HYPLAYER_TEMPLATEBASEFILTER_H
#define HYPLAYER_TEMPLATEBASEFILTER_H


#include <GLES2/gl2.h>
#include "../base/matrix_util.h"
#include "../image/ImageCreator.h"

static GLfloat vertex[] = {
        1.0f, 1.0f,
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
};

static GLfloat texture[] = {
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

class TemplateBaseFilter {

public:

    TemplateBaseFilter();

    virtual ~TemplateBaseFilter();

    void doFrame();

private:

    GLuint textureId = 0;
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint program = 0;

    const GLchar *aPosition = "aPosition";
    const GLchar *aTextureCoordinate = "aTextureCoordinate";
    const GLchar *uTextureMatrix = "uTextureMatrix";
    const GLchar *uMVPMatrix = "uMVPMatrix";
    const GLchar *uTextureSampler = "uTextureSampler";

    GLint aPositionLocation = 0;
    GLint aTextureCoordinateLocation = 0;
    GLint uTextureMatrixLocation = 0;
    GLint uMVPMatrixLocation = 0;
    GLint uTextureSamplerLocation = 0;

    ESMatrix *uCoordMatrix;
    ESMatrix *textureMatrix;

    ImageCreator *imageCreator;
};


#endif //HYPLAYER_TEMPLATEBASEFILTER_H
