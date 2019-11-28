//
// Created by templechen on 2019-11-27.
//

#include <base/gl_utils.h>
#include <image/ImageCreator.h>
#include "ObjViewerCubeTextureFilter.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static const char *VERTEX_SHADER = GET_STR(
        attribute highp vec3 myVertex;
        attribute highp vec3 myNormal;
        attribute mediump vec2 myUV;
        attribute mediump vec4 myBone;
        varying lowp vec4 colorDiffuse;
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
            highp vec3 worldNormal = vec3(mat3(uMVMatrix[0].xyz, uMVMatrix[1].xyz, uMVMatrix[2].xyz) * myNormal);
            highp vec3 ecPosition = p.xyz;
            // brighten the teapot color by 50%
            colorDiffuse = clamp(vMaterialDiffuse * 1.5,
                                 vec4(0.0, 0.0, 0.0, 0.0),
                                 vec4(1.0, 1.0, 1.0, 1.0));
            normal = worldNormal;
            position = ecPosition;
        }
);

static const char *FRAGMENT_SHADER = GET_STR(
        uniform lowp vec3 vMaterialAmbient;
        uniform lowp vec4 vMaterialSpecular;
        uniform samplerCube samplerObj;
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
            vec3 texCoord = normal;
            texCoord.y = -normal.y;
            gl_FragColor = colorDiffuse * textureCube(samplerObj, normalize(texCoord)) +
                           vec4(vMaterialAmbient.xyz, 1.0);
        }
);

void ObjViewerCubeTextureFilter::initShaders() {
    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
}

ObjViewerCubeTextureFilter::ObjViewerCubeTextureFilter() {

}

void ObjViewerCubeTextureFilter::init() {
    ObjViewerFilter::init();

    assert(files.size() == 6);

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
    assert(texId != GL_INVALID_VALUE);

    for(GLuint i = 0; i < 6; i++) {
        stbi_set_flip_vertically_on_load(1);
        int w, h, comp;
        unsigned char* image =
                stbi_load(files[i], &w, &h, &comp, STBI_default);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, GL_RGBA,
                     w, h,
                     0, GL_RGBA,
                     GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT );
    glActiveTexture(GL_TEXTURE0);

    GLint sampler = glGetUniformLocation(shaderProgram->program,
                                         "samplerObj");
    glUniform1i(sampler, 0);

    glBindTexture(GL_TEXTURE0, texId);
    glActiveTexture(GL_TEXTURE0);
}

void ObjViewerCubeTextureFilter::release() {
    ObjViewerFilter::release();
    if(texId != GL_INVALID_VALUE) {
        glDeleteBuffers(1, &texId);
        texId = GL_INVALID_VALUE;
    }
}
