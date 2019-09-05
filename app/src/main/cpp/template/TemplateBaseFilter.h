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

    void setNativeWindowSize(int width, int height);

    void doFrame();

    void release();

    int time = 0;

private:

    GLuint yTexture;
    GLuint uTexture;
    GLuint vTexture;
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint program = 0;

    const GLchar *aPosition = "aPosition";
    const GLchar *aTextureCoordinate = "aTextureCoordinate";
    const GLchar *uTextureMatrix = "uTextureMatrix";
    const GLchar *uMVPMatrix = "uMVPMatrix";
    const GLchar *uTextureY = "uTextureY";
    const GLchar *uTextureU = "uTextureU";
    const GLchar *uTextureV = "uTextureV";

    GLint aPositionLocation = 0;
    GLint aTextureCoordinateLocation = 0;
    GLint uTextureMatrixLocation = 0;
    GLint uMVPMatrixLocation = 0;

    GLint uTextureYLocation = -1;
    GLint uTextureULocation = -1;
    GLint uTextureVLocation = -1;

    ESMatrix baseMVPMatrix;
    ESMatrix textureMatrix;
    void updateMatrix();

    ImageCreator *imageCreator;

    //size
    int windowWidth;
    int windowHeight;
    int imageWidth;
    int imageHeight;
    float originScaleX;
    float originScaleY;

    //
    float scaleX = 1.0f;
    float scaleY = 1.0f;
};


#endif //HYPLAYER_TEMPLATEBASEFILTER_H
