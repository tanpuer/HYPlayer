//
// Created by templechen on 2019-04-28.
//

#include <base/gl_utils.h>
#include "base_filter.h"

extern "C" {
#include <libavutil/frame.h>
}

base_filter::base_filter() {

}

base_filter::~base_filter() {
    releaseTextures();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);
}

void base_filter::init_program() {
    initVertexShader();
    initFragmentShader();
    vertexShader = loadShader(GL_VERTEX_SHADER, vertex_shader_string);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragment_shader_string);
    program = createShaderProgram(vertexShader, fragmentShader);
    initMatrix();
    createTextures();
}

void base_filter::drawFrame(AVFrame *avFrame) {
    checkVideoSize(avFrame);
    glUseProgram(program);

    GLint vertexCount = sizeof(videoVertex) / (sizeof(videoVertex[0]) * 2);
    aPositionLocation = glGetAttribLocation(program, aPosition);
    glEnableVertexAttribArray(aPositionLocation);
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 8, videoVertex);

    aTextureCoordinateLocation = glGetAttribLocation(program, aTextureCoordinate);
    glEnableVertexAttribArray(aTextureCoordinateLocation);
    glVertexAttribPointer(aTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 8, videoTexture);

    uTextureMatrixLocation = glGetUniformLocation(program, uTextureMatrix);
    glUniformMatrix4fv(uTextureMatrixLocation, 1, GL_FALSE, this->textureMatrix->m);

    uCoordMatrixLocation = glGetUniformLocation(program, uCoordinateMatrix);
    glUniformMatrix4fv(uCoordMatrixLocation, 1, GL_FALSE, this->uCoordMatrix->m);

    //
    drawTextures(avFrame);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTextureCoordinateLocation);
    glBindTexture(GL_TEXTURE_2D, 0);

    av_frame_free(&avFrame);
}

void base_filter::initMatrix() {
    textureMatrix = new ESMatrix();
    textureMatrix->m[0] = 1.0f;
    textureMatrix->m[1] = 0.0f;
    textureMatrix->m[2] = 0.0f;
    textureMatrix->m[3] = 0.0f;
    textureMatrix->m[4] = 0.0f;
    textureMatrix->m[5] = -1.0f;
    textureMatrix->m[6] = 0.0f;
    textureMatrix->m[7] = 0.0f;
    textureMatrix->m[8] = 0.0f;
    textureMatrix->m[9] = 0.0f;
    textureMatrix->m[10] = 1.0f;
    textureMatrix->m[11] = 0.0f;
    textureMatrix->m[12] = 0.0f;
    textureMatrix->m[13] = 1.0f;
    textureMatrix->m[14] = 0.0f;
    textureMatrix->m[15] = 1.0f;
}

void base_filter::checkVideoSize(AVFrame *frame) {
    int width = frame->width;
    int height = frame->height;
    if (this->width != width || this->height != height) {
        this->width = width;
        this->height = height;
        initCoordMatrix();
        if (screen_width > 0 && screen_height > 0) {
            float screen_ratio = screen_width * 1.0f / screen_height;
            float video_ratio = width * 1.0f / height;
            if (video_ratio > screen_ratio) {
                scaleM(uCoordMatrix, 0, 1.0f, screen_ratio / video_ratio, 1.0f);
            } else {
                scaleM(uCoordMatrix, 0, 1.0f, video_ratio / screen_ratio, 1.0f);
            }
        }
    }
}

void base_filter::initCoordMatrix() {
    uCoordMatrix = new ESMatrix();
    setIdentityM(uCoordMatrix);
}

void base_filter::initVertexShader() {
    vertex_shader_string = {
            "uniform mat4 uTextureMatrix;\n"
            "attribute vec4 aPosition;\n"
            "attribute vec4 aTextureCoordinate;\n"
            "varying vec2 vTextureCoord;\n"
            "uniform mat4 uCoordMatrix;\n"
            "void main()\n"
            "{\n"
            "    vTextureCoord = (uTextureMatrix * aTextureCoordinate).xy;\n"
            "    gl_Position = uCoordMatrix * aPosition;\n"
            "}\n"
    };
}

static const char *fragYUV420P = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTextureCoord;     //顶点着色器传递的坐标
        uniform sampler2D uTextureY; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uTextureU;
        uniform sampler2D uTextureV;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(uTextureY,vTextureCoord).r;
            yuv.g = texture2D(uTextureU,vTextureCoord).r - 0.5;
            yuv.b = texture2D(uTextureV,vTextureCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                      0.0,-0.39465,2.03211,
                      1.13983,-0.58060,0.0)*yuv;
            gl_FragColor = vec4(rgb,1.0);
        }
);

void base_filter::initFragmentShader() {
//    fragment_shader_string= {
//        "precision highp float;\n"
//        "uniform sampler2D uTextureY;\n"
//        "uniform sampler2D uTextureU;\n"
//        "uniform sampler2D uTextureV;\n"
//        "varying vec2 vTextureCoord;\n"
//        "void main()\n"
//        "{\n"
//        "float y = texture2D(uTextureY, vTextureCoord).r;\n"
//        "float u = texture2D(uTextureU, vTextureCoord).r - 0.5;\n"
//        "float v = texture2D(uTextureV, vTextureCoord).r - 0.5;\n"
//        "float r = y + 1.402 * v;\n"
//        "float g = y - 0.344 * u - 0.714 * v;\n"
//        "float b = y + 1.772 * u;\n"
//        "gl_FragColor = vec4(r,g,b,1.0);\n"
//        "}\n"
//    };
    fragment_shader_string = fragYUV420P;
}

void base_filter::createTextures() {
    yTexture = createTexture(GL_TEXTURE_2D);
    uTexture = createTexture(GL_TEXTURE_2D);
    vTexture = createTexture(GL_TEXTURE_2D);
}

void base_filter::drawTextures(AVFrame *avFrame) {
    //pay attention to linsize and width, maybe they are not the same

    uTextureYLocation = glGetUniformLocation(program, uTextureY);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, yTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, avFrame->linesize[0], avFrame->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                 avFrame->data[0]);
    glUniform1i(uTextureYLocation, 0);

    uTextureULocation = glGetUniformLocation(program, uTextureU);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, uTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, avFrame->linesize[1], avFrame->height / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 avFrame->data[1]);
    glUniform1i(uTextureULocation, 1);

    uTextureVLocation = glGetUniformLocation(program, uTextureV);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, vTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, avFrame->linesize[2], avFrame->height / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 avFrame->data[2]);
    glUniform1i(uTextureVLocation, 2);
}

void base_filter::releaseTextures() {
    auto textures = new GLuint[3];
    textures[0] = yTexture;
    textures[1] = uTexture;
    textures[2] = vTexture;
    glDeleteTextures(3, textures);
}

