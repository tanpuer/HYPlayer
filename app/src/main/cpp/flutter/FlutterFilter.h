//
// Created by templechen on 2019-09-22.
//

#ifndef HYPLAYER_FLUTTERFILTER_H
#define HYPLAYER_FLUTTERFILTER_H


#include <GLES2/gl2.h>
#include <base/matrix_util.h>
#include <image/IAVFrameCreator.h>
#include <gpu/GrContext.h>
#include <core/SkSurface.h>

//static GLfloat vertex[] = {
//        1.0f, 1.0f,
//        -1.0f, 1.0f,
//        -1.0f, -1.0f,
//        1.0f, 1.0f,
//        -1.0f, -1.0f,
//        1.0f, -1.0f,
//};
//
//static GLfloat texture[] = {
//        1.0f, 1.0f,
//        0.0f, 1.0f,
//        0.0f, 0.0f,
//        1.0f, 1.0f,
//        0.0f, 0.0f,
//        1.0f, 0.0f
//};

class FlutterFilter {

public:

    FlutterFilter();

    virtual ~FlutterFilter();

    virtual void setNativeWindowSize(int width, int height);

    virtual void doFrame();

    virtual void release();

    int time = 0;

protected:

    void genFrameBuffer(int width, int height);

    GLuint frameBufferTextureId = 0;
    GLuint frameBuffer = 0;
    GLuint fboVertexShader = 0;
    GLuint fboFragmentShader = 0;
    GLuint fboProgram = 0;

    const char *fboPosition = "fboPosition";
    const char *fboTextureMatrix = "fboTextureMatrix";
    const char *fboTextureCoordinate = "fboTextureCoordinate";
    const char *fboTexture = "fboTexture";

    GLint fboPositionLocation = 0;
    GLint fboTextureMatrixLocation = 0;
    GLint fboTextureCoordinateLocation = 0;
    GLint fboTextureLocation = 0;

    ESMatrix fboTexMatrix;

    sk_sp<GrContext> context = nullptr;
    sk_sp<SkSurface> skia_surface = nullptr;

    int windowWidth;
    int windowHeight;
};


#endif //HYPLAYER_FLUTTERFILTER_H
