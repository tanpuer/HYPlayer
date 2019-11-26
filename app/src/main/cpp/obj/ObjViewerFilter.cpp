//
// Created by templechen on 2019-11-26.
//

#include <base/gl_utils.h>
#include "ObjViewerFilter.h"
#include "teapot.inl"

static const char *VERTEX_SHADER = GET_STR(
        attribute highp vec3    myVertex;
        attribute highp vec3    myNormal;
        attribute mediump vec2  myUV;
        attribute mediump vec4  myBone;
        varying mediump vec2    texCoord;
        varying lowp    vec4    colorDiffuse;
        varying mediump vec3 position;
        varying mediump vec3 normal;
        uniform highp mat4      uMVMatrix;
        uniform highp mat4      uPMatrix;
        uniform highp vec3      vLight0;
        uniform lowp vec4       vMaterialDiffuse;
        uniform lowp vec3       vMaterialAmbient;
        uniform lowp vec4       vMaterialSpecular;
        void main(void) {
            highp vec4 p = vec4(myVertex,1);
            gl_Position = uPMatrix * p;
            texCoord = myUV;
            highp vec3 worldNormal = vec3(mat3(uMVMatrix[0].xyz, uMVMatrix[1].xyz, uMVMatrix[2].xyz) * myNormal);
            highp vec3 ecPosition = p.xyz;
            colorDiffuse = dot( worldNormal, normalize(-vLight0+ecPosition) ) * vMaterialDiffuse  + vec4( vMaterialAmbient, 1 );
            normal = worldNormal;
            position = ecPosition;
       }
);

static const char *FRAGMEMT_SHADER = GET_STR(
        uniform lowp vec3 vMaterialAmbient;
        uniform lowp vec4 vMaterialSpecular;
        varying lowp vec4 colorDiffuse;
        uniform highp vec3 vLight0;
        varying mediump vec3 position;
        varying mediump vec3 normal;
        void main() {
            mediump vec3 halfVector = normalize(-vLight0 + position);
            mediump float NdotH = max(dot(normalize(normal), halfVector), 0.0);
            mediump float fPower = vMaterialSpecular.w;
            mediump float specular = pow(NdotH, fPower);
            lowp vec4 colorSpecular = vec4( vMaterialSpecular.xyz * specular, 1 );
            gl_FragColor = colorDiffuse + colorSpecular;
        }
);

ObjViewerFilter::ObjViewerFilter() {
    glFrontFace(GL_CCW);

    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMEMT_SHADER);

    GLuint program;
    GLint linked;
    program = glCreateProgram();
    if (program == 0) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return ;
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Bind attribute locations
    // this needs to be done prior to linking
    glBindAttribLocation(program, ATTRIB_VERTEX, "myVertex");
    glBindAttribLocation(program, ATTRIB_NORMAL, "myNormal");
    glBindAttribLocation(program, ATTRIB_UV, "myUV");

    // 链接program程序
    glLinkProgram(program);
    // 检查链接状态
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        // 检查日志信息长度
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            // 分配一个足以存储日志信息的字符串
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            // 检索日志信息
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            ALOGE("Error linking program:\n%s\n", infoLog);
            // 使用完成后需要释放字符串分配的内存
            free(infoLog);
        }
        // 删除着色器释放内存
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        return ;
    }

    // Get uniform locations
    shader_param_ = new SHADER_PARAMS();
    shader_param_->matrix_projection_ = glGetUniformLocation(program, "uPMatrix");
    shader_param_->matrix_view_ = glGetUniformLocation(program, "uMVMatrix");

    shader_param_->light0_ = glGetUniformLocation(program, "vLight0");
    shader_param_->material_diffuse_ = glGetUniformLocation(program, "vMaterialDiffuse");
    shader_param_->material_ambient_ = glGetUniformLocation(program, "vMaterialAmbient");
    shader_param_->material_specular_ =
            glGetUniformLocation(program, "vMaterialSpecular");

    shader_param_->program_ = program;

    // Create Index buffer
    num_indices_ = sizeof(teapotIndices) / sizeof(teapotIndices[0]);
    glGenBuffers(1, &ibo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapotIndices), teapotIndices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create VBO
    num_vertices_ = sizeof(teapotPositions) / sizeof(teapotPositions[0]) / 3;
    int32_t stride = sizeof(TEAPOT_VERTEX);
    int32_t index = 0;
    TEAPOT_VERTEX* p = new TEAPOT_VERTEX[num_vertices_];
    for (int32_t i = 0; i < num_vertices_; ++i) {
        p[i].pos[0] = teapotPositions[index];
        p[i].pos[1] = teapotPositions[index + 1];
        p[i].pos[2] = teapotPositions[index + 2];

        p[i].normal[0] = teapotNormals[index];
        p[i].normal[1] = teapotNormals[index + 1];
        p[i].normal[2] = teapotNormals[index + 2];
        index += 3;
    }
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, stride * num_vertices_, p, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] p;

    mat_model_ = ndk_helper::Mat4::Translation(0, 0, -15.f);

    ndk_helper::Mat4 mat = ndk_helper::Mat4::RotationX(M_PI / 3);
    mat_model_ = mat * mat_model_;
}

void ObjViewerFilter::updateViewport() {
    // Init Projection matrices
    int32_t viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    const float CAM_NEAR = 5.f;
    const float CAM_FAR = 10000.f;
    if (viewport[2] < viewport[3]) {
        float aspect =
                static_cast<float>(viewport[2]) / static_cast<float>(viewport[3]);
        mat_projection_ =
                ndk_helper::Mat4::Perspective(aspect, 1.0f, CAM_NEAR, CAM_FAR);
    } else {
        float aspect =
                static_cast<float>(viewport[3]) / static_cast<float>(viewport[2]);
        mat_projection_ =
                ndk_helper::Mat4::Perspective(1.0f, aspect, CAM_NEAR, CAM_FAR);
    }
}

void ObjViewerFilter::release() {
    if (vbo_) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }

    if (ibo_) {
        glDeleteBuffers(1, &ibo_);
        ibo_ = 0;
    }

    if (shader_param_->program_) {
        glDeleteProgram(shader_param_->program_);
        shader_param_->program_ = 0;
    }
}

void ObjViewerFilter::doFrame() {
    mat_view_ = ndk_helper::Mat4::LookAt(ndk_helper::Vec3(CAM_X, CAM_Y, CAM_Z),
                                         ndk_helper::Vec3(0.f, 0.f, 0.f),
                                         ndk_helper::Vec3(0.f, 1.f, 0.f));
    mat_view_ = mat_view_ * mat_model_;


    //
    // Feed Projection and Model View matrices to the shaders
    ndk_helper::Mat4 mat_vp = mat_projection_ * mat_view_;

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    int32_t iStride = sizeof(TEAPOT_VERTEX);
    // Pass the vertex data
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, iStride,
                          BUFFER_OFFSET(0));
    glEnableVertexAttribArray(ATTRIB_VERTEX);

    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, iStride,
                          BUFFER_OFFSET(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(ATTRIB_NORMAL);

    // Bind the IB
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

    glUseProgram(shader_param_->program_);

    TEAPOT_MATERIALS material = {
            {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 10.f}, {0.1f, 0.1f, 0.1f}, };

    // Update uniforms
    glUniform4f(shader_param_->material_diffuse_, material.diffuse_color[0],
                material.diffuse_color[1], material.diffuse_color[2], 1.f);

    glUniform4f(shader_param_->material_specular_, material.specular_color[0],
                material.specular_color[1], material.specular_color[2],
                material.specular_color[3]);
    //
    // using glUniform3fv here was troublesome
    //
    glUniform3f(shader_param_->material_ambient_, material.ambient_color[0],
                material.ambient_color[1], material.ambient_color[2]);

    glUniformMatrix4fv(shader_param_->matrix_projection_, 1, GL_FALSE,
                       mat_vp.Ptr());
    glUniformMatrix4fv(shader_param_->matrix_view_, 1, GL_FALSE, mat_view_.Ptr());
    glUniform3f(shader_param_->light0_, 100.f, -200.f, -600.f);

    glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_SHORT,
                   BUFFER_OFFSET(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ObjViewerFilter::setNativeWindowSize(int width, int height) {
    updateViewport();
}
