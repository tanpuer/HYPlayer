//
// Created by templechen on 2019-04-28.
//

#ifndef FFMPEG_PLAYER_BASE_FILTER_H
#define FFMPEG_PLAYER_BASE_FILTER_H

#include <GLES3/gl3.h>
#include <base/matrix_util.h>

extern "C" {
#include <libavutil/frame.h>
};

static GLfloat videoVertex[] = {
        1.0f, 1.0f,
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
};

static GLfloat videoTexture[] = {
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

#define GET_STR(x) #x

class base_filter {

public:
    base_filter();

    virtual ~base_filter();

    virtual void init_program();

    void drawFrame(AVFrame *avFrame);

    int screen_width = 0;
    int screen_height = 0;

protected:

    int width;
    int height;

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    const GLchar *aPosition = "aPosition";
    const GLchar *aTextureCoordinate = "aTextureCoordinate";
    const GLchar *uTextureMatrix = "uTextureMatrix";
    const GLchar *uTextureY = "uTextureY";
    const GLchar *uTextureU = "uTextureU";
    const GLchar *uTextureV = "uTextureV";
    const GLchar *uCoordinateMatrix = "uCoordMatrix";
    GLint aPositionLocation = -1.0;
    GLint aTextureCoordinateLocation = -1.0;
    GLint uTextureMatrixLocation = -1.0;
    GLint uTextureYLocation = -1.0;
    GLint uTextureULocation = -1.0;
    GLint uTextureVLocation = -1.0;
    GLint uCoordMatrixLocation = -1.0;

    virtual void initVertexShader();

    virtual void initFragmentShader();

    virtual void createTextures();

    virtual void releaseTextures();

    virtual void drawTextures(AVFrame *avFrame);

    const char *vertex_shader_string;

    const char *fragment_shader_string;

    ESMatrix *textureMatrix;

    void initMatrix();

    GLuint yTexture;
    GLuint uTexture;
    GLuint vTexture;

    void checkVideoSize(AVFrame *frame);

    ESMatrix *uCoordMatrix;

    void initCoordMatrix();
};

#endif //FFMPEG_PLAYER_BASE_FILTER_H
