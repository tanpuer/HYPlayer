//
// Created by templechen on 2019-09-02.
//

#include "TemplateBaseFilter.h"
#include "../image/ImageCreator.h"
#include "../base/matrix_util.h"
#include "../base/gl_utils.h"

#define GET_STR(x) #x

static const char *VERTEX_SHADER_STR = GET_STR(
    attribute vec4 aPosition;
    uniform mat4 uTextureMatrix;
    attribute vec4 aTextureCoordinate;
    varying vec2 vTextureCoord;
    uniform mat4 uMVPMatrix;
    void main()
    {
        vTextureCoord = (uTextureMatrix * aTextureCoordinate).xy;
        gl_Position = uMVPMatrix * aPosition;
    }
);

static const char *FRAGMENT_SHADER_STR = GET_STR(
    precision mediump float;
    uniform sampler2D uTextureSampler;
    varying vec2 vTextureCoord;
    void main()
    {
        gl_FragColor = texture2D(uTextureSampler, vTextureCoord);
    }
);

TemplateBaseFilter::TemplateBaseFilter() {
    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_STR);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_STR);
    program = createShaderProgram(vertexShader, fragmentShader);
    textureId = createTexture(GL_TEXTURE_2D);
    ALOGD("shader: %d %d; program: %d; texture: %d", vertexShader, fragmentShader, program, textureId);

     imageCreator = new ImageCreator();

    uCoordMatrix = new ESMatrix();
    setIdentityM(uCoordMatrix);
    textureMatrix = new ESMatrix();
    setIdentityM(textureMatrix);
}

TemplateBaseFilter::~TemplateBaseFilter() {
    delete imageCreator;

    glDeleteProgram(program);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    auto textures = new GLuint[1];
    textures[0] = textureId;
    glDeleteTextures(1, textures);
}

void TemplateBaseFilter::doFrame() {
    glUseProgram(program);

    GLint vertexCount = sizeof(vertex) / (sizeof(vertex[0]) * 2);
    aPositionLocation = glGetAttribLocation(program, aPosition);
    glEnableVertexAttribArray(aPositionLocation);
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 8, vertex);

    aTextureCoordinateLocation = glGetAttribLocation(program, aTextureCoordinate);
    glEnableVertexAttribArray(aTextureCoordinateLocation);
    glVertexAttribPointer(aTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 8, texture);

    uTextureMatrixLocation = glGetUniformLocation(program, uTextureMatrix);
    glUniformMatrix4fv(uTextureMatrixLocation, 1, GL_FALSE, this->textureMatrix->m);

    uMVPMatrixLocation = glGetUniformLocation(program, uMVPMatrix);
    glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, this->uCoordMatrix->m);

    uTextureSamplerLocation = glGetUniformLocation(program, uTextureSampler);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    AVFrame *avFrame = imageCreator->readImage("sdcard/test.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, avFrame->width, avFrame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 avFrame->data[0]);
    glUniform1i(uTextureSamplerLocation, 0);

    ALOGD("aPositionLocation: %d; "
          "aTextureCoordinateLocation : %d; "
          "uTextureMatrixLocation : %d "
          "uMVPMatrixLocation: %d "
          "uTextureSamplerLocation: %d",
            aPositionLocation, aTextureCoordinateLocation, uTextureMatrixLocation, uMVPMatrixLocation, uTextureSamplerLocation)

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTextureCoordinateLocation);
    glBindTexture(GL_TEXTURE_2D, 0);
}
