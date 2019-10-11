//
// Created by templechen on 2019-10-11.
//

#ifndef HYPLAYER_CAMERABASEFILTER_H
#define HYPLAYER_CAMERABASEFILTER_H


#include <GLES3/gl3.h>
#include <base/matrix_util.h>

static GLfloat cameraVertex[] = {
        1.0f, 1.0f,
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
};

static GLfloat cameraTexture[] = {
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

class CameraBaseFilter {

public:

    CameraBaseFilter();

    ~CameraBaseFilter();

    void setNativeWindowSize(int width, int height);

    void release();

    void doFrame();

    virtual void readShaderString();

public:
    GLuint oesTextureId;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    const GLchar *aPosition = "aPosition";
    const GLchar *aTextureCoordinate = "aTextureCoordinate";
    const GLchar *uTextureMatrix = "uTextureMatrix";
    const GLchar *uCoordMatrix = "uCoordMatrix";
    GLint aPositionLocation = -1.0;
    GLint aTextureCoordinateLocation = -1.0;
    GLint uTextureMatrixLocation = -1.0;
    GLint uCoordMatrixLocation = -1.0;

    ESMatrix *textureMatrix;
    ESMatrix *coordMatrix;

private:

    const char *vertex_shader_string = nullptr;
    const char *fragment_shader_string = nullptr;

    int videoWidth = 0;
    int videoHeight = 0;
    int viewWidth = 0;
    int viewHeight = 0;

//    GLfloat myVertex[12] = {
//            1.0f, 1.0f,
//            -1.0f, 1.0f,
//            -1.0f, -1.0f,
//            1.0f, 1.0f,
//            -1.0f, -1.0f,
//            1.0f, -1.0f,
//    };


};


#endif //HYPLAYER_CAMERABASEFILTER_H
