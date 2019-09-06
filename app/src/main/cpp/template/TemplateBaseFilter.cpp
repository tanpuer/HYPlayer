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
    precision highp float;
    varying vec2 vTextureCoord;
    uniform sampler2D uTextureY;
    uniform sampler2D uTextureU;
    uniform sampler2D uTextureV;
    void main()
    {
        float y = texture2D(uTextureY, vTextureCoord).r;
        float u = texture2D(uTextureU, vTextureCoord).r - 0.5;
        float v = texture2D(uTextureV, vTextureCoord).r - 0.5;
        float r = y + 1.402 * v;
        float g = y - 0.344 * u - 0.714 * v;
        float b = y + 1.772 * u;
        gl_FragColor = vec4(r,g,b,1.0);
    }
);

TemplateBaseFilter::TemplateBaseFilter() {
    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_STR);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_STR);
    program = createShaderProgram(vertexShader, fragmentShader);
    yTexture = createTexture(GL_TEXTURE_2D);
    uTexture = createTexture(GL_TEXTURE_2D);
    vTexture = createTexture(GL_TEXTURE_2D);

    avFrameCreator = new ImageCreator();

    baseMVPMatrix = ESMatrix();
    textureMatrix = ESMatrix();
    setIdentityM(&baseMVPMatrix);
    setIdentityM(&textureMatrix);
    textureMatrix.m[5] = -1.0f;
    textureMatrix.m[13] = 1.0f;
}

TemplateBaseFilter::~TemplateBaseFilter() {
    avFrameCreator->releaseFrame();
    delete avFrameCreator;
    ALOGD("delete TemplateBaseFilter");
}

void TemplateBaseFilter::doFrame() {
    AVFrame *avFrame = avFrameCreator->readFrame("sdcard/test.jpeg");
    if (avFrame == nullptr) {
        ALOGE("doFrame with avFrame nullptr, pls check");
        return;
    }
    if (avFrame->width != imageWidth || avFrame->height != imageHeight) {
        imageWidth = avFrame->width;
        imageHeight = avFrame->height;
        updateMatrix();
    }

    //test start..................................................
    scaleX += 0.002;
    scaleY = scaleX;
    updateMatrix();
    //test end..................................................

    glUseProgram(program);

    GLint vertexCount = sizeof(vertex) / (sizeof(vertex[0]) * 2);
    aPositionLocation = glGetAttribLocation(program, aPosition);
    glEnableVertexAttribArray(aPositionLocation);
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 8, vertex);

    aTextureCoordinateLocation = glGetAttribLocation(program, aTextureCoordinate);
    glEnableVertexAttribArray(aTextureCoordinateLocation);
    glVertexAttribPointer(aTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 8, texture);

    uTextureMatrixLocation = glGetUniformLocation(program, uTextureMatrix);
    glUniformMatrix4fv(uTextureMatrixLocation, 1, GL_FALSE, this->textureMatrix.m);

    uMVPMatrixLocation = glGetUniformLocation(program, uMVPMatrix);
    glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, this->baseMVPMatrix.m);

    uTextureYLocation = glGetUniformLocation(program, uTextureY);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, yTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, avFrame->width, avFrame->height, 0,
                 GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 avFrame->data[0]);
    glUniform1i(uTextureYLocation, 0);

    uTextureULocation = glGetUniformLocation(program, uTextureU);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, uTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, avFrame->width / 2, avFrame->height / 2, 0,
                 GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 avFrame->data[1]);
    glUniform1i(uTextureULocation, 1);

    uTextureVLocation = glGetUniformLocation(program, uTextureV);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, vTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, avFrame->width / 2, avFrame->height / 2, 0,
                 GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 avFrame->data[2]);
    glUniform1i(uTextureVLocation, 2);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTextureCoordinateLocation);
    glBindTexture(GL_TEXTURE_2D, 0);

    time++;
}

void TemplateBaseFilter::setNativeWindowSize(int width, int height) {
    this->windowWidth = width;
    this->windowHeight = height;
    updateMatrix();
}

void TemplateBaseFilter::updateMatrix() {
    if (windowWidth > 0 && windowHeight > 0 && imageWidth > 0 && imageHeight > 0) {
        if (true) {
            //fit_center
            if (imageWidth * 1.0 / imageHeight > windowWidth * 1.0 / windowHeight) {
                //横屏视频
                originScaleY = windowWidth * 1.0F / imageWidth * imageHeight / windowHeight;
                originScaleX = 1.0F;
            } else {
                //竖屏视频
                originScaleY = 1.0F;
                originScaleX = windowHeight * 1.0F / imageHeight * imageWidth / windowWidth;
            }
        } else {
            //center_crop
            if (imageWidth * 1.0 / imageHeight > windowWidth * 1.0 / windowHeight) {
                //横屏视频
                originScaleY = 1.0F;
                originScaleX = windowHeight * 1.0F / imageHeight * imageWidth / windowWidth;
            } else {
                //竖屏视频
                originScaleY = windowWidth * 1.0F / imageWidth * imageHeight / windowHeight;
                originScaleX = 1.0F;
            }
        }

        setIdentityM(&baseMVPMatrix);
        scaleM(&baseMVPMatrix, 0, originScaleX * scaleX, originScaleY * scaleY, 1.0F);
    }
}

void TemplateBaseFilter::release() {
    ALOGD("TemplateBaseFilter release");
    glDeleteProgram(program);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    auto textures = new GLuint[3];
    textures[0] = yTexture;
    textures[1] = uTexture;
    textures[2] = vTexture;
    glDeleteTextures(3, textures);
}
