//
// Created by templechen on 2019-11-26.
//

#include <base/gl_utils.h>
#include <iostream>
#include "ObjViewerFilter.h"
#include "teapot.inl"
#include "timerutil.h"

static const char *VERTEX_SHADER = GET_STR(
        attribute highp vec3 myVertex;
        attribute highp vec3 myNormal;
        attribute mediump vec2 myUV;
        attribute mediump vec4 myBone;
        varying mediump vec2 texCoord;
        varying lowp vec4 diffuseLight;
        varying mediump vec3 position;
        varying mediump vec3 normal;
        uniform highp mat4 uMVMatrix;
        uniform highp mat4 uPMatrix;
        uniform highp vec3 vLight0;
        uniform lowp vec4 vMaterialDiffuse;
        uniform lowp vec3 vMaterialAmbient;
        uniform lowp vec4 vMaterialSpecular;
        void main(void) {
            highp vec4 p = vec4(myVertex,1);
            gl_Position = uPMatrix * p;
            texCoord = myUV;
            highp vec3 worldNormal = vec3(mat3(uMVMatrix[0].xyz, uMVMatrix[1].xyz, uMVMatrix[2].xyz) * myNormal);
            highp vec3 ecPosition = p.xyz;
            diffuseLight = dot( worldNormal, normalize(-vLight0+ecPosition)) * vec4(1.0, 1.0, 1.0, 1.0);
            normal = worldNormal;
            position = ecPosition;
        }
);

static const char *FRAGMEMT_SHADER = GET_STR(
        uniform lowp vec3 vMaterialAmbient;
        uniform lowp vec4 vMaterialSpecular;
        uniform sampler2D samplerObj;
        varying mediump vec2 texCoord;
        varying lowp vec4 diffuseLight;
        uniform highp vec3   vLight0;
        varying mediump vec3 position;
        varying mediump vec3 normal;
        void main() {
            mediump vec3 halfVector = normalize(-vLight0 + position);
            mediump float NdotH = max(dot(normalize(normal), halfVector), 0.0);
            mediump float fPower = vMaterialSpecular.w;
            mediump float specular = pow(NdotH, fPower);
            lowp vec4 colorSpecular = vec4( vMaterialSpecular.xyz * specular, 1 );
            // increase ambient light to brighten the teapot :-)
//            gl_FragColor = diffuseLight * texture2D(samplerObj, texCoord) +
//            2.0f * vec4(vMaterialAmbient.xyz, 1.0f) + colorSpecular;
            gl_FragColor = texture2D(samplerObj, texCoord);
        }
);

ObjViewerFilter::ObjViewerFilter() {
    loadObj();
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
        projectionMatrix =
                ndk_helper::Mat4::Perspective(aspect, 1.0f, CAM_NEAR, CAM_FAR);
    } else {
        float aspect =
                static_cast<float>(viewport[3]) / static_cast<float>(viewport[2]);
        projectionMatrix =
                ndk_helper::Mat4::Perspective(1.0f, aspect, CAM_NEAR, CAM_FAR);
    }
}

void ObjViewerFilter::release() {

    if (!vbos.empty()) {
        for (int i=0; i< vbos.size(); ++i) {
            glDeleteBuffers(1, &vbos[i]);
        }
    }
    if (!textures.empty()) {
        for (int i=0; i< textures.size(); ++i) {
            glDeleteBuffers(1, &textures[i]);
        }
    }
    if (!texture2Ds.empty()) {
        for (int i=0; i< texture2Ds.size(); ++i) {
            delete texture2Ds[i];
        }
    }

    if (shaderProgram->program) {
        glDeleteProgram(shaderProgram->program);
        shaderProgram->program = 0;
    }

}

void ObjViewerFilter::doFrame() {
    viewMatrix = ndk_helper::Mat4::LookAt(ndk_helper::Vec3(CAM_X, CAM_Y, CAM_Z / scaleIndex),
                                          ndk_helper::Vec3(0.f, 0.f, 0.f),
                                          ndk_helper::Vec3(0.f, 1.f, 0.f));

    modelMatrix = ndk_helper::Mat4::Identity();
    ndk_helper::Mat4 scrollXMat = ndk_helper::Mat4::RotationY(-PI * scrollX / 180);
    ndk_helper::Mat4 scrollYMat = ndk_helper::Mat4::RotationX(-PI * scrollY / 180);
    modelMatrix = scrollXMat * scrollYMat * modelMatrix;

    viewMatrix = viewMatrix * modelMatrix;

    //
    // Feed Projection and Model View matrices to the shaders
    ndk_helper::Mat4 mat_vp = projectionMatrix * viewMatrix;

    glUseProgram(shaderProgram->program);

    TEAPOT_MATERIALS material = {
            {1.0f, 0.5f, 0.5f},
            {1.0f, 1.0f, 1.0f, 10.f},
            {0.1f, 0.1f, 0.1f},};

    // Update uniforms
    glUniform4f(shaderProgram->materialDiffuse, material.diffuseColor[0],
                material.diffuseColor[1], material.diffuseColor[2], 1.f);

    glUniform4f(shaderProgram->materialSpecular, material.specularColor[0],
                material.specularColor[1], material.specularColor[2],
                material.specularColor[3]);
    //
    // using glUniform3fv here was troublesome
    //
    glUniform3f(shaderProgram->materialAmbient, material.ambientColor[0],
                material.ambientColor[1], material.ambientColor[2]);

    glUniformMatrix4fv(shaderProgram->projectionMatrix, 1, GL_FALSE,
                       mat_vp.Ptr());
    glUniformMatrix4fv(shaderProgram->viewMatrix, 1, GL_FALSE, viewMatrix.Ptr());
    glUniform3f(shaderProgram->light0, 100.f, -200.f, -6000.f);

    for (int i = 0; i < shapes.size(); ++i) {

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

        int32_t iStride = sizeof(TEAPOT_VERTEX);
        // Pass the vertex data
        glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, iStride,
                              BUFFER_OFFSET(0));
        glEnableVertexAttribArray(ATTRIB_VERTEX);

        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, iStride,
                              BUFFER_OFFSET(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(ATTRIB_NORMAL);

        glBindBuffer(GL_ARRAY_BUFFER, textures[i]);

        texture2Ds[i]->bindTexture();

        glUniform1i(shaderProgram->samplerObj, i);

        glVertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, GL_FALSE,
                              2 * sizeof(GLfloat),
                              BUFFER_OFFSET(0));
        glEnableVertexAttribArray(ATTRIB_UV);

        glDrawArrays(GL_TRIANGLES, 0, vertices[i]);

        //    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT,
//                   BUFFER_OFFSET(0));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

void ObjViewerFilter::setNativeWindowSize(int width, int height) {
    updateViewport();
}

void ObjViewerFilter::initShaders() {
    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMEMT_SHADER);
}

void ObjViewerFilter::init() {
    initShaders();

    GLuint program;
    GLint linked;
    program = glCreateProgram();
    if (program == 0) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Bind attribute locations
    // this needs to be done prior to linking
    // 将通用顶点索引与命名属性变量相关联
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
        return;
    }

    // Get uniform locations
    // 返回统一变量的位置
    shaderProgram = new SHADER_PARAMS();
    shaderProgram->projectionMatrix = glGetUniformLocation(program, "uPMatrix");
    shaderProgram->viewMatrix = glGetUniformLocation(program, "uMVMatrix");

    shaderProgram->light0 = glGetUniformLocation(program, "vLight0");
    shaderProgram->materialDiffuse = glGetUniformLocation(program, "vMaterialDiffuse");
    shaderProgram->materialAmbient = glGetUniformLocation(program, "vMaterialAmbient");
    shaderProgram->materialSpecular =
            glGetUniformLocation(program, "vMaterialSpecular");
    shaderProgram->samplerObj = glGetUniformLocation(program, "samplerObj");

    shaderProgram->program = program;

    int32_t stride = sizeof(TEAPOT_VERTEX);
    bool hasNormal = attrib.normals.size() > 0;
    vbos = std::vector<GLuint>(shapes.size());
    vertices = std::vector<int>(shapes.size());
    textures = std::vector<GLuint>(shapes.size());
    texture2Ds = std::vector<Texture2D*>(shapes.size());
    for (int i = 0; i < shapes.size(); ++i) {
        int num = shapes[i].mesh.indices.size();
        vertices[i] = num;
        TEAPOT_VERTEX *p = new TEAPOT_VERTEX[num];
        std::vector<float> coords;
        int32_t index = 0;

        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[i].mesh.num_face_vertices[f];
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
                p[index].pos[0] = attrib.vertices[3 * idx.vertex_index + 0];
                p[index].pos[1] = attrib.vertices[3 * idx.vertex_index + 1];
                p[index].pos[2] = attrib.vertices[3 * idx.vertex_index + 2];
                if (hasNormal) {
                    p[index].normal[0] = attrib.normals[3 * idx.normal_index + 0];
                    p[index].normal[1] = attrib.normals[3 * idx.normal_index + 1];
                    p[index].normal[2] = attrib.normals[3 * idx.normal_index + 2];
                }

                coords.push_back(attrib.texcoords[2*idx.texcoord_index+0]);
                coords.push_back(attrib.texcoords[2*idx.texcoord_index+1]);

                index++;
            }
            index_offset += fv;
        }

        glGenBuffers(1, &(vbos[i]));
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, stride * num, p, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        delete[] p;

        //texture
        glGenBuffers(1, &(textures[i]));
        glBindBuffer(GL_ARRAY_BUFFER, textures[i]);
        glBufferData(GL_ARRAY_BUFFER,
                     2 * sizeof(float) * num,
                     coords.data(), GL_STATIC_DRAW);
        tinyobj::material_t* mp = &materials[i];
        const char *path = ("sdcard/" + mp->diffuse_texname).c_str();
        texture2Ds[i] = new Texture2D(path);
        texture2Ds[i]->create();
        ALOGD("path is %s %d %d", path, texture2Ds[i]->texId, coords.size());

    }

    modelMatrix = ndk_helper::Mat4::Translation(0, 0, -15.f);
}

void ObjViewerFilter::loadObj() {
    timerutil t;
    t.start();
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
//                                "/sdcard/usemtl-issue-68.obj",
//                                "/sdcard/Handgun_obj.obj",
//                                "/sdcard/issue-138.obj",
//                                "/sdcard/12228_Dog_v1_L2.obj",
//                                "/sdcard/teapot_n_glass.obj",
                                "/sdcard/batman.obj",
                                "/sdcard", true);
    t.end();
    if (!warn.empty()) {
        ALOGD("%s", warn.c_str());
    }
    if (!err.empty()) {
        ALOGD("%s", err.c_str());
    }
    if (!ret) {
        ALOGD("Failed to load/parse .obj!");
        return;
    }
    ALOGD("Parsing obj success, time: %lu [msecs]\n", t.msec());
}

void ObjViewerFilter::scroll(int scrollX, int scrollY) {
    this->scrollX = scrollX;
    this->scrollY = scrollY;
}

void ObjViewerFilter::scale(float scale) {
    this->scaleIndex = scale / 1000000;
}
