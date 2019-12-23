//
// Created by templechen on 2019-11-26.
//

#ifndef HYPLAYER_OBJVIEWERFILTER_H
#define HYPLAYER_OBJVIEWERFILTER_H

#include <GLES3/gl3.h>
#include <base/matrix_util.h>
#include "vecmath.h"
#include "tiny_obj_loader.h"
#include "Texture2D.h"

#define GET_STR(x) #x
#define BUFFER_OFFSET(i) ((char*)NULL + (i))

struct SHADER_VERTEX {
    float pos[3];
    float normal[3];
};

enum SHADER_ATTRIBUTES {
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    ATTRIB_UV,
    ATTRIB_COLOR
};

struct SHADER_PARAMS {
    GLuint program;
    GLuint light0;
    GLuint materialDiffuse;
    GLuint materialAmbient;
    GLuint materialSpecular;

    GLuint projectionMatrix;
    GLuint viewMatrix;

    GLuint samplerObj;
    GLuint diffuse;
};

struct SHADER_MATERIALS {
    float diffuseColor[3];
    float specularColor[4];
    float ambientColor[3];
};

class ObjViewerFilter {

public:

    ObjViewerFilter();

    virtual void init();

    virtual void release();

    virtual void doFrame();

    void setNativeWindowSize(int width, int height);

    void scroll(int scrollX, int scrollY);

    void scale(float scale);

    virtual void loadObj();

    virtual void initShaders();

protected:

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    ndk_helper::Mat4 projectionMatrix;
    ndk_helper::Mat4 viewMatrix;
    ndk_helper::Mat4 modelMatrix;
    SHADER_PARAMS *shaderProgram;

    void updateViewport();

    float CAM_X = 0.f;
    float CAM_Y = 0.f;
    float CAM_Z = 70.f;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<GLuint> vbos;
    std::vector<int> vertices;
    std::vector<GLuint> textures;
    std::vector<Texture2D*> texture2Ds;
    std::map<std::string, int>texturesMap;

    int scrollX = 0;
    int scrollY = 0;

    float scaleIndex = 1.0f;
};


#endif //HYPLAYER_OBJVIEWERFILTER_H
