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
                vec2 vecRight = vec2(vTextureCoord.x / 2.0 + 0.5, vTextureCoord.y);
                float y = texture2D(uTextureY, vecLeft).r;
                float u = texture2D(uTextureU, vecLeft).r - 0.5;
                float v = texture2D(uTextureV, vecLeft).r - 0.5;
                float r = y + 1.402 * v;
                float g = y - 0.344 * u - 0.714 * v;
                float b = y + 1.772 * u;
                vec3 colorLeft = vec3(r,g,b);

               float y1 = texture2D(uTextureY, vecRight).r;
               float u1 = texture2D(uTextureU, vecRight).r - 0.5;
               float v1 = texture2D(uTextureV, vecRight).r - 0.5;
               float alpha = y1 + 1.402 * v1;
               float g1 = y1 - 0.344 * u1 - 0.714 * v1;
               float b1 = y1 + 1.772 * u1;
                gl_FragColor = vec4(colorLeft.rgb, alpha);
//                gl_FragColor = vec4(alpha, g1, b1, 1.0);
           }
);

AlphaBaseFilter::AlphaBaseFilter() {
    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_STR);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_STR);
    program = createShaderProgram(vertexShader, fragmentShader);
    yTexture = createTexture(GL_TEXTURE_2D);
    uTexture = createTexture(GL_TEXTURE_2D);
    vTexture = createTexture(GL_TEXTURE_2D);

//    avFrameCreator = new ImageCreator("sdcard/test.jpeg");
//    avFrameCreator = new GifCreator("sdcard/test.gif");
//    avFrameCreator = new VideoCreator("sdcard/trailer111.mp4");
    avFrameCreator = new VideoCreator("sdcard/trailer_with_alpha.mp4");

    baseMVPMatrix = ESMatrix();
    textureMatrix = ESMatrix();
    setIdentityM(&baseMVPMatrix);
    setIdentityM(&textureMatrix);
    textureMatrix.m[5] = -1.0f;
    textureMatrix.m[13] = 1.0f;
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
