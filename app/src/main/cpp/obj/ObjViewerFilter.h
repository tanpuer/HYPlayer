//
// Created by templechen on 2019-11-26.
//

#ifndef HYPLAYER_OBJVIEWERFILTER_H
#define HYPLAYER_OBJVIEWERFILTER_H

#include <GLES3/gl3.h>
#include <base/matrix_util.h>
#include "vecmath.h"

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
    GLuint program_;
    GLuint light0_;
    GLuint material_diffuse_;
    GLuint material_ambient_;
    GLuint material_specular_;

    GLuint matrix_projection_;
    GLuint matrix_view_;
};

struct TEAPOT_MATERIALS {
    float diffuse_color[3];
    float specular_color[4];
    float ambient_color[3];
};

class ObjViewerFilter {

public:

    ObjViewerFilter();

    void release();

    void doFrame();

    void setNativeWindowSize(int width, int height);

private:

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    int32_t num_indices_;
    int32_t num_vertices_;
    GLuint ibo_;
    GLuint vbo_;

    ndk_helper::Mat4 mat_projection_;
    ndk_helper::Mat4 mat_view_;
    ndk_helper::Mat4 mat_model_;
    SHADER_PARAMS *shader_param_;

    void updateViewport();
};


#endif //HYPLAYER_OBJVIEWERFILTER_H
