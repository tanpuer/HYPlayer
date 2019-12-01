//
// Created by templechen on 2019-11-26.
//

#ifndef HYPLAYER_OBJVIEWERFILTER_H
#define HYPLAYER_OBJVIEWERFILTER_H

#include <GLES3/gl3.h>
#include <base/matrix_util.h>
#include "vecmath.h"
#include "tiny_obj_loader.h"

#define GET_STR(x) #x
#define BUFFER_OFFSET(i) ((char*)NULL + (i))

struct TEAPOT_VERTEX {
    float pos[3];
    float normal[3];
};

enum SHADER_ATTRIBUTES {
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    ATTRIB_UV,
};

struct SHADER_PARAMS {
    GLuint program;
    GLuint light0;
    GLuint materialDiffuse;
    GLuint materialAmbient;
    GLuint materialSpecular;

    GLuint projectionMatrix;
    GLuint viewMatrix;
};

struct TEAPOT_MATERIALS {
    float diffuseColor[3];
    float specularColor[4];
    float ambientColor[3];
};

class ObjViewerFilter {

public:

    ObjViewerFilter();

    virtual void init();

    virtual void release();

    void doFrame();

    void setNativeWindowSize(int width, int height);

protected:

    virtual void initShaders();

    float* getTextureCoords();

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    int32_t numIndices;
    int32_t numVertices = 0;
    GLuint ibo;
    GLuint vbo;

    ndk_helper::Mat4 projectionMatrix;
    ndk_helper::Mat4 viewMatrix;
    ndk_helper::Mat4 modelMatrix;
    SHADER_PARAMS *shaderProgram;

    void updateViewport();

    float CAM_X = 0.f;
    float CAM_Y = 0.f;
    float CAM_Z = 30.f;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    void loadObj();
};


#endif //HYPLAYER_OBJVIEWERFILTER_H
