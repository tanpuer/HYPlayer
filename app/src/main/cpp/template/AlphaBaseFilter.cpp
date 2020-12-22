//
// Created by DZSB-000848 on 2020/9/19.
//

#include <image/VideoCreator.h>
#include <base/gl_utils.h>
#include <obj/NewObjFilter.h>
#include "AlphaBaseFilter.h"

#define GET_STR(x) #x

static const char *VERTEX_SHADER_STR = GET_STR(
        attribute vec4 aPosition;
           uniform mat4 uTextureMatrix;
           attribute vec4 aTextureCoordinate;
           varying vec2 vTextureCoord;
           uniform mat4 uMVPMatrix;
           void main()
           {
               vec4 vec = (uTextureMatrix * aTextureCoordinate);
               vTextureCoord = vec2(vec.x, vec.y);
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
                vec2 vecLeft = vec2(vTextureCoord.x / 2.0, vTextureCoord.y);
                float y = texture2D(uTextureY, vecLeft).r;
                float u = texture2D(uTextureU, vecLeft).r - 0.5;
                float v = texture2D(uTextureV, vecLeft).r - 0.5;
                float r = y + 1.402 * v;
                float g = y - 0.344 * u - 0.714 * v;
                float b = y + 1.772 * u;
                vec3 colorLeft = vec3(r,g,b);

                vec2 vecRight = vec2(vTextureCoord.x / 2.0 + 0.5, vTextureCoord.y);
                float y1 = texture2D(uTextureY, vecRight).r;
                float v1 = texture2D(uTextureV, vecRight).r - 0.5;
                float alpha = y1 + 1.402 * v1;
                gl_FragColor = vec4(colorLeft.rgb, alpha);
           }
);

AlphaBaseFilter::AlphaBaseFilter() {

}

void AlphaBaseFilter::initGLProgram() {
    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_STR);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_STR);
    program = createShaderProgram(vertexShader, fragmentShader);
    yTexture = createTexture(GL_TEXTURE_2D);
    uTexture = createTexture(GL_TEXTURE_2D);
    vTexture = createTexture(GL_TEXTURE_2D);

    avFrameCreator = new VideoCreator("sdcard/trailer_with_alpha.mp4");

    baseMVPMatrix = ESMatrix();
    textureMatrix = ESMatrix();
    setIdentityM(&baseMVPMatrix);
    setIdentityM(&textureMatrix);
    textureMatrix.m[5] = -1.0f;
    textureMatrix.m[13] = 1.0f;

    //init alpha
    initTransPrograms();
}

AlphaBaseFilter::~AlphaBaseFilter() {

}

void AlphaBaseFilter::updateMatrix() {
    if (windowWidth > 0 && windowHeight > 0 && imageWidth > 0 && imageHeight > 0) {
        int realWidth = imageWidth / 2;
        if (true) {
            //fit_center
            if (realWidth /2 * 1.0 / imageHeight > windowWidth * 1.0 / windowHeight) {
                //横屏视频
                originScaleY = windowWidth * 1.0F / realWidth * imageHeight / windowHeight;
                originScaleX = 1.0F;
            } else {
                //竖屏视频
                originScaleY = 1.0F;
                originScaleX = windowHeight * 1.0F / imageHeight * realWidth / windowWidth;
            }
        } else {
            //center_crop
            if (realWidth * 1.0 / imageHeight > windowWidth * 1.0 / windowHeight) {
                //横屏视频
                originScaleY = 1.0F;
                originScaleX = windowHeight * 1.0F / imageHeight * realWidth / windowWidth;
            } else {
                //竖屏视频
                originScaleY = windowWidth * 1.0F / realWidth * imageHeight / windowHeight;
                originScaleX = 1.0F;
            }
        }

        setIdentityM(&baseMVPMatrix);
        scaleM(&baseMVPMatrix, 0, originScaleX * scaleX, originScaleY * scaleY, 1.0F);
    }
}

static const char *VERTEX_TRANS_SHADER_STR = GET_STR(
        attribute vec4 aTransPosition;
        void main() {
            gl_Position = aTransPosition;
        }
);

static const char *FRAGMENT_TRANS_SHADER_STR = GET_STR(
        precision highp float;
        void main() {
            gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);
        }
);

void AlphaBaseFilter::doFrame() {
    glUseProgram(transProgram);
    GLint vertexCount = sizeof(vertex) / (sizeof(vertex[0]) * 2);
    aTransPositionLocation = glGetAttribLocation(transProgram, "aTransPosition");
    glEnableVertexAttribArray(aTransPositionLocation);
    glVertexAttribPointer(aTransPositionLocation, 2, GL_FLOAT, GL_FALSE, 8, vertex);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(aTransPositionLocation);

    TemplateBaseFilter::doFrame();
}

void AlphaBaseFilter::initTransPrograms() {
    transVertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_TRANS_SHADER_STR);
    transFragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_TRANS_SHADER_STR);
    transProgram = createShaderProgram(transVertexShader, transFragmentShader);
}

void AlphaBaseFilter::releaseTransProgram() {
    glDeleteProgram(transProgram);
    glDeleteShader(transVertexShader);
    glDeleteShader(transFragmentShader);
}

void AlphaBaseFilter::release() {
    releaseTransProgram();
    TemplateBaseFilter::release();
}
