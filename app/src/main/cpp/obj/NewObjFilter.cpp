//
// Created by templechen on 2019-12-20.
//

#include <base/native_log.h>
#include <base/gl_utils.h>
#include "NewObjFilter.h"
#include "timerutil.h"
#include "stb_image.h"

static const char *VERTEX_SHADER = GET_STR(
        attribute highp vec3 myVertex;
        attribute highp vec3 myNormal;
        attribute highp vec2 myUV;
        attribute mediump vec4 myBone;
        varying highp vec2 texCoord;
        varying lowp vec4 diffuseLight;
        varying mediump vec3 position;
        varying mediump vec3 normal;
        uniform highp mat4 uMVMatrix;
        uniform highp mat4 uPMatrix;
        uniform highp vec3 vLight0;
        uniform lowp vec4 vMaterialDiffuse;
        uniform lowp vec3 vMaterialAmbient;
        uniform lowp vec4 vMaterialSpecular;
        attribute highp vec3 myColor;
        varying highp vec3 vMyColor;
        void main(void) {
            vMyColor = myColor;
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
        varying highp vec2 texCoord;
        varying lowp vec4 diffuseLight;
        uniform highp vec3   vLight0;
        varying mediump vec3 position;
        varying mediump vec3 normal;
        varying highp vec3 vMyColor;
        void main() {
            mediump vec3 halfVector = normalize(-vLight0 + position);
            mediump float NdotH = max(dot(normalize(normal), halfVector), 0.0);
            mediump float fPower = vMaterialSpecular.w;
            mediump float specular = pow(NdotH, fPower);
            lowp vec4 colorSpecular = vec4( vMaterialSpecular.xyz * specular, 1 );
            // increase ambient light to brighten the teapot :-)
//            gl_FragColor = diffuseLight * texture2D(samplerObj, texCoord) +
//            2.0f * vec4(vMaterialAmbient.xyz, 1.0f) + colorSpecular;
//            gl_FragColor = texture2D(samplerObj, texCoord);
//            gl_FragColor = vec4(vMyColor, 1.0);
            gl_FragColor = mix(texture2D(samplerObj, texCoord), vec4(vMyColor, 1.0), 0.2);
        }
);

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
    float v10[3];
    v10[0] = v1[0] - v0[0];
    v10[1] = v1[1] - v0[1];
    v10[2] = v1[2] - v0[2];

    float v20[3];
    v20[0] = v2[0] - v0[0];
    v20[1] = v2[1] - v0[1];
    v20[2] = v2[2] - v0[2];

    N[0] = v20[1] * v10[2] - v20[2] * v10[1];
    N[1] = v20[2] * v10[0] - v20[0] * v10[2];
    N[2] = v20[0] * v10[1] - v20[1] * v10[0];

    float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
    if (len2 > 0.0f) {
        float len = sqrtf(len2);

        N[0] /= len;
        N[1] /= len;
        N[2] /= len;
    }
}

static void normalizeVector(vec3 &v) {
    float len2 = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
    if (len2 > 0.0f) {
        float len = sqrtf(len2);

        v.v[0] /= len;
        v.v[1] /= len;
        v.v[2] /= len;
    }
}

void NewObjFilter::loadObj() {
    timerutil t;
    t.start();
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
//                                "/sdcard/12228_Dog_v1_L2.obj",
//                                "/sdcard/teapot_n_glass.obj",
//                                "/sdcard/batman.obj",
//                                "/sdcard/earth.obj",
//                                "/sdcard/A380.obj",
//                                "/sdcard/uh60.obj",
//                                "/sdcard/LAM.obj",
//                                "/sdcard/IronMan.obj",
//                                "/sdcard/Ak-47.obj",
//                                "/sdcard/Japanese_Temple.obj",
                                "/sdcard/11803_Airplane_v1_l1.obj",
//                                "/sdcard/tobao.obj",
//                                "/sdcard/Touareg.obj",
//                                "/sdcard/Airbus_A310.obj",
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
    ALOGD("shape and material size is %d %d", shapes.size(), materials.size())
    ALOGD("Parsing obj success, time: %lu [msecs]\n", t.msec());

    bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
    bmax[0] = bmax[1] = bmax[2] = -std::numeric_limits<float>::max();

    LoadObjAndConvert();

    maxExtent = 0.5f * (bmax[0] - bmin[0]);
    if (maxExtent < 0.5f * (bmax[1] - bmin[1])) {
        maxExtent = 0.5f * (bmax[1] - bmin[1]);
    }
    if (maxExtent < 0.5f * (bmax[2] - bmin[2])) {
        maxExtent = 0.5f * (bmax[2] - bmin[2]);
    }
}

bool NewObjFilter::hasSmoothingGroup(const tinyobj::shape_t& shape)
{
    for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++) {
        if (shape.mesh.smoothing_group_ids[i] > 0) {
            return true;
        }
    }
    return false;
}

void NewObjFilter::computeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape,
                             std::map<int, vec3>& smoothVertexNormals) {
    smoothVertexNormals.clear();
    std::map<int, vec3>::iterator iter;

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        // Get the three indexes of the face (all faces are triangular)
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        // Get the three vertex indexes and coordinates
        int vi[3];      // indexes
        float v[3][3];  // coordinates

        for (int k = 0; k < 3; k++) {
            vi[0] = idx0.vertex_index;
            vi[1] = idx1.vertex_index;
            vi[2] = idx2.vertex_index;
            assert(vi[0] >= 0);
            assert(vi[1] >= 0);
            assert(vi[2] >= 0);

            v[0][k] = attrib.vertices[3 * vi[0] + k];
            v[1][k] = attrib.vertices[3 * vi[1] + k];
            v[2][k] = attrib.vertices[3 * vi[2] + k];
        }

        // Compute the normal of the face
        float normal[3];
        CalcNormal(normal, v[0], v[1], v[2]);

        // Add the normal to the three vertexes
        for (size_t i = 0; i < 3; ++i) {
            iter = smoothVertexNormals.find(vi[i]);
            if (iter != smoothVertexNormals.end()) {
                // add
                iter->second.v[0] += normal[0];
                iter->second.v[1] += normal[1];
                iter->second.v[2] += normal[2];
            } else {
                smoothVertexNormals[vi[i]].v[0] = normal[0];
                smoothVertexNormals[vi[i]].v[1] = normal[1];
                smoothVertexNormals[vi[i]].v[2] = normal[2];
            }
        }

    }  // f

    // Normalize the normals, that is, make them unit vectors
    for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end();
         iter++) {
        normalizeVector(iter->second);
    }

}

bool NewObjFilter::LoadObjAndConvert() {
    // Append `default` material
    materials.push_back(tinyobj::material_t());

    {
        for (size_t m = 0; m < materials.size(); m++) {
            tinyobj::material_t *mp = &materials[m];

            if (mp->diffuse_texname.length() > 0) {
                // Only load the texture if it is not already loaded
                if (texturesIdMap.find(mp->diffuse_texname) == texturesIdMap.end()) {
                    GLuint texture_id;
                    int w, h;
                    int comp;

                    timerutil t;
                    t.start();
//                    stbi_set_flip_vertically_on_load(1);
                    unsigned char *image =
                            stbi_load(("sdcard/" + mp->diffuse_texname).c_str(), &w, &h, &comp, STBI_default);

                    t.end();
                    ALOGD("Parsing time:%s %lu [msecs] %d %d %d\n", ("sdcard/" + mp->diffuse_texname).c_str(),  t.msec(), w, h, comp);

                    glGenTextures(1, &texture_id);
                    glActiveTexture(GL_TEXTURE0 + texture_id - 1);
                    glBindTexture(GL_TEXTURE_2D, texture_id);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    int format = 0;
                    if (comp == 1) {
                        format = GL_LUMINANCE;
                    } else if (comp == 2) {
                        format = GL_LUMINANCE_ALPHA;
                    } else if (comp == 3) {
                        format = GL_RGB;
                    } else if (comp == 4) {
                        format = GL_RGBA;
                    } else {
                        //todo
                        ALOGE("unSupport type %d %s", comp, mp->diffuse_texname.data());
                    }
                    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format,
                                 GL_UNSIGNED_BYTE, image);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    stbi_image_free(image);
                    texturesIdMap.insert(std::make_pair(mp->diffuse_texname, texture_id));
                }
            }
        }
    }

    {
        for (size_t s = 0; s < shapes.size(); s++) {
            DrawObject o;
            std::vector<float> buffer;  // pos(3float), normal(3float), color(3float)

            // Check for smoothing group and compute smoothing normals
            std::map<int, vec3> smoothVertexNormals;
            if (hasSmoothingGroup(shapes[s]) > 0) {
                computeSmoothingNormals(attrib, shapes[s], smoothVertexNormals);
            }

            for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
                tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
                tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
                tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

                int current_material_id = shapes[s].mesh.material_ids[f];

                if ((current_material_id < 0) ||
                    (current_material_id >= static_cast<int>(materials.size()))) {
                    // Invaid material ID. Use default material.
                    current_material_id =
                            materials.size() -
                            1;  // Default material is added to the last item in `materials`.
                }
                float diffuse[3];
                for (size_t i = 0; i < 3; i++) {
                    diffuse[i] = materials[current_material_id].diffuse[i];
                }
                float tc[3][2];
                if (attrib.texcoords.size() > 0) {
                    if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
                        (idx2.texcoord_index < 0)) {
                        // face does not contain valid uv index.
                        tc[0][0] = 0.0f;
                        tc[0][1] = 0.0f;
                        tc[1][0] = 0.0f;
                        tc[1][1] = 0.0f;
                        tc[2][0] = 0.0f;
                        tc[2][1] = 0.0f;
                    } else {
                        assert(attrib.texcoords.size() >
                               size_t(2 * idx0.texcoord_index + 1));
                        assert(attrib.texcoords.size() >
                               size_t(2 * idx1.texcoord_index + 1));
                        assert(attrib.texcoords.size() >
                               size_t(2 * idx2.texcoord_index + 1));

                        // Flip Y coord.
                        tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
                        tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
                        tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
                        tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
                        tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
                        tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
                    }
                } else {
                    tc[0][0] = 0.0f;
                    tc[0][1] = 0.0f;
                    tc[1][0] = 0.0f;
                    tc[1][1] = 0.0f;
                    tc[2][0] = 0.0f;
                    tc[2][1] = 0.0f;
                }

                float v[3][3];
                for (int k = 0; k < 3; k++) {
                    int f0 = idx0.vertex_index;
                    int f1 = idx1.vertex_index;
                    int f2 = idx2.vertex_index;
                    assert(f0 >= 0);
                    assert(f1 >= 0);
                    assert(f2 >= 0);

                    v[0][k] = attrib.vertices[3 * f0 + k];
                    v[1][k] = attrib.vertices[3 * f1 + k];
                    v[2][k] = attrib.vertices[3 * f2 + k];
                    bmin[k] = std::min(v[0][k], bmin[k]);
                    bmin[k] = std::min(v[1][k], bmin[k]);
                    bmin[k] = std::min(v[2][k], bmin[k]);
                    bmax[k] = std::max(v[0][k], bmax[k]);
                    bmax[k] = std::max(v[1][k], bmax[k]);
                    bmax[k] = std::max(v[2][k], bmax[k]);
                }

                float n[3][3];
                {
                    bool invalid_normal_index = false;
                    if (attrib.normals.size() > 0) {
                        int nf0 = idx0.normal_index;
                        int nf1 = idx1.normal_index;
                        int nf2 = idx2.normal_index;

                        if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) {
                            // normal index is missing from this face.
                            invalid_normal_index = true;
                        } else {
                            for (int k = 0; k < 3; k++) {
                                assert(size_t(3 * nf0 + k) < attrib.normals.size());
                                assert(size_t(3 * nf1 + k) < attrib.normals.size());
                                assert(size_t(3 * nf2 + k) < attrib.normals.size());
                                n[0][k] = attrib.normals[3 * nf0 + k];
                                n[1][k] = attrib.normals[3 * nf1 + k];
                                n[2][k] = attrib.normals[3 * nf2 + k];
                            }
                        }
                    } else {
                        invalid_normal_index = true;
                    }

                    if (invalid_normal_index && !smoothVertexNormals.empty()) {
                        // Use smoothing normals
                        int f0 = idx0.vertex_index;
                        int f1 = idx1.vertex_index;
                        int f2 = idx2.vertex_index;

                        if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
                            n[0][0] = smoothVertexNormals[f0].v[0];
                            n[0][1] = smoothVertexNormals[f0].v[1];
                            n[0][2] = smoothVertexNormals[f0].v[2];

                            n[1][0] = smoothVertexNormals[f1].v[0];
                            n[1][1] = smoothVertexNormals[f1].v[1];
                            n[1][2] = smoothVertexNormals[f1].v[2];

                            n[2][0] = smoothVertexNormals[f2].v[0];
                            n[2][1] = smoothVertexNormals[f2].v[1];
                            n[2][2] = smoothVertexNormals[f2].v[2];

                            invalid_normal_index = false;
                        }
                    }

                    if (invalid_normal_index) {
                        // compute geometric normal
                        CalcNormal(n[0], v[0], v[1], v[2]);
                        n[1][0] = n[0][0];
                        n[1][1] = n[0][1];
                        n[1][2] = n[0][2];
                        n[2][0] = n[0][0];
                        n[2][1] = n[0][1];
                        n[2][2] = n[0][2];
                    }
                }

                for (int k = 0; k < 3; k++) {
                    buffer.push_back(v[k][0]);
                    buffer.push_back(v[k][1]);
                    buffer.push_back(v[k][2]);
                    buffer.push_back(n[k][0]);
                    buffer.push_back(n[k][1]);
                    buffer.push_back(n[k][2]);
                    // Combine normal and diffuse to get color.
//                    float normal_factor = 1.0;
//                    float diffuse_factor = 1 - normal_factor;
//                    float c[3] = {n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
//                                  n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
//                                  n[k][2] * normal_factor + diffuse[2] * diffuse_factor};
//                    float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
//                    if (len2 > 0.0f) {
//                        float len = sqrtf(len2);
//                        c[0] /= len;
//                        c[1] /= len;
//                        c[2] /= len;
//                    }
//                    buffer.push_back(c[0] * 0.5 + 0.5);
//                    buffer.push_back(c[1] * 0.5 + 0.5);
//                    buffer.push_back(c[2] * 0.5 + 0.5);

                    buffer.push_back(diffuse[0]);
                    buffer.push_back(diffuse[1]);
                    buffer.push_back(diffuse[2]);

                    buffer.push_back(tc[k][0]);
                    buffer.push_back(tc[k][1]);
                }
            }

            o.vb_id = 0;
            o.numTriangles = 0;

            // OpenGL viewer does not support texturing with per-face material.
            if (shapes[s].mesh.material_ids.size() > 0 &&
                shapes[s].mesh.material_ids.size() > s) {
                o.material_id = shapes[s].mesh.material_ids[0];  // use the material ID
                // of the first face.
            } else {
                o.material_id = materials.size() - 1;  // = ID for default material.
            }
            ALOGD("shape[%d] material_id %d\n", int(s), int(o.material_id));

            if (buffer.size() > 0) {
                glGenBuffers(1, &o.vb_id);
                glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
                glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float),
                             &buffer.at(0), GL_STATIC_DRAW);
                o.numTriangles = buffer.size() / (3 + 3 + 3 + 2) /
                                 3;  // 3:vtx, 3:normal, 3:col, 2:texcoord

                ALOGD("shape[%d] # of triangles = %d\n", static_cast<int>(s),
                       o.numTriangles);
            }

            (&drawObjects)->push_back(o);
        }
    }

    ALOGD("bmin = %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
    ALOGD("bmax = %f, %f, %f\n", bmax[0], bmax[1], bmax[2]);

    return true;
}

void NewObjFilter::doFrame() {

    viewMatrix = ndk_helper::Mat4::LookAt(
            ndk_helper::Vec3(CAM_X / maxExtent, CAM_Y / maxExtent, CAM_Z / scaleIndex / maxExtent),
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

    SHADER_MATERIALS material = {
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

        DrawObject o = drawObjects[i];
        glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);

        glBindTexture(GL_TEXTURE_2D, 0);
        if ((o.material_id < materials.size())) {
            std::string diffuse_texname = materials[o.material_id].diffuse_texname;
            if (texturesIdMap.find(diffuse_texname) != texturesIdMap.end()) {
                glActiveTexture(GL_TEXTURE0 + texturesIdMap[diffuse_texname] - 1);
                glBindTexture(GL_TEXTURE_2D, texturesIdMap[diffuse_texname]);
                glUniform1i(shaderProgram->samplerObj, texturesIdMap[diffuse_texname] - 1);
            }
        }

        int32_t iStride = (3 + 3 + 3 + 2) * sizeof(float);;
        // Pass the vertex data
        glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, iStride,
                              BUFFER_OFFSET(0));
        glEnableVertexAttribArray(ATTRIB_VERTEX);

        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, iStride,
                              BUFFER_OFFSET(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(ATTRIB_NORMAL);

        glVertexAttribPointer(ATTRIB_COLOR, 3 ,GL_FLOAT, GL_FALSE, iStride, BUFFER_OFFSET(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(ATTRIB_COLOR);

        glVertexAttribPointer(ATTRIB_UV, 2 ,GL_FLOAT, GL_FALSE, iStride, BUFFER_OFFSET(9 * sizeof(GLfloat)));
        glEnableVertexAttribArray(ATTRIB_UV);

        glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);

        //    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT,
//                   BUFFER_OFFSET(0));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }



}

void NewObjFilter::init() {
    loadObj();

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
    glBindAttribLocation(program, ATTRIB_COLOR, "myColor");

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
    shaderProgram->diffuse = glGetUniformLocation(program, "diffuse");

    shaderProgram->program = program;
}

void NewObjFilter::initShaders() {
    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMEMT_SHADER);
}
