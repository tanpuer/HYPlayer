//
// Created by templechen on 2019-10-11.
//

#include <base/gl_utils.h>
#include "CameraBaseFilter.h"

CameraBaseFilter::CameraBaseFilter() {
    oesTextureId = createTexture(GL_TEXTURE_EXTERNAL_OES);

    textureMatrix = new ESMatrix();
    setIdentityM(textureMatrix);
    coordMatrix = new ESMatrix();
    updateMatrix();

    readShaderString();
    vertexShader = loadShader(GL_VERTEX_SHADER, vertex_shader_string);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragment_shader_string);
    program = createShaderProgram(vertexShader, fragmentShader);

}

CameraBaseFilter::~CameraBaseFilter() {

}

void CameraBaseFilter::release() {
    auto textures = new GLuint[1];
    textures[0] = oesTextureId;
    glDeleteTextures(1, textures);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);

    delete textureMatrix;
    delete coordMatrix;
}

void CameraBaseFilter::doFrame() {
    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, oesTextureId);

    GLint vertexCount = sizeof(cameraVertex) / (sizeof(cameraVertex[0]) * 2);
    aPositionLocation = glGetAttribLocation(program, aPosition);
    glEnableVertexAttribArray(aPositionLocation);
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 8, cameraVertex);

    aTextureCoordinateLocation = glGetAttribLocation(program, aTextureCoordinate);
    glEnableVertexAttribArray(aTextureCoordinateLocation);
    glVertexAttribPointer(aTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 8, cameraTexture);

    uTextureMatrixLocation = glGetUniformLocation(program, uTextureMatrix);
    glUniformMatrix4fv(uTextureMatrixLocation, 1, GL_FALSE, this->textureMatrix->m);

    uCoordMatrixLocation = glGetUniformLocation(program, uCoordMatrix);
    glUniformMatrix4fv(uCoordMatrixLocation, 1, GL_FALSE, this->coordMatrix->m);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTextureCoordinateLocation);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
}

void CameraBaseFilter::readShaderString() {
    vertex_shader_string = {
            "uniform mat4 uTextureMatrix;\n"
            "uniform mat4 uCoordMatrix;\n"
            "attribute vec4 aPosition;\n"
            "attribute vec4 aTextureCoordinate;\n"
            "varying vec2 vTextureCoord;\n"
            "void main()\n"
            "{\n"
            "    vTextureCoord = (uTextureMatrix * aTextureCoordinate).xy;\n"
            "    gl_Position = uCoordMatrix * aPosition;\n"
            "}\n"
    };
    fragment_shader_string = {
            "#extension GL_OES_EGL_image_external : require\n"
            "precision mediump float;\n"
            "uniform samplerExternalOES uTextureSampler;\n"
            "varying vec2 vTextureCoord;\n"
            "void main()\n"
            "{\n"
            "    gl_FragColor = texture2D(uTextureSampler, vTextureCoord);\n"
            "}\n"
    };
}

void CameraBaseFilter::setVideoSize(int width, int height) {
    this->viewWidth = width;
    this->viewHeight = height;
    updateMatrix();
}

void CameraBaseFilter::setNativeWindowSize(int width, int height) {
    this->videoWidth = width;
    this->videoHeight = height;
    updateMatrix();
}

void CameraBaseFilter::updateMatrix() {
    if (videoWidth > 0 && videoHeight > 0 && viewWidth > 0 && viewHeight > 0) {
        if (true) {
            //fit_center
            if (videoWidth * 1.0 / videoHeight > viewWidth * 1.0 / viewHeight) {
                //横屏视频
                originScaleY = viewWidth * 1.0F / videoWidth * videoHeight / viewHeight;
                originScaleX = 1.0F;
            } else {
                //竖屏视频
                originScaleY = 1.0F;
                originScaleX = viewHeight * 1.0F / videoHeight * videoWidth / viewWidth;
            }
        } else {
            //center_crop
            if (videoWidth * 1.0 / videoHeight > viewWidth * 1.0 / viewHeight) {
                //横屏视频
                originScaleY = 1.0F;
                originScaleX = viewHeight * 1.0F / videoHeight * videoWidth / viewWidth;
            } else {
                //竖屏视频
                originScaleY = viewWidth * 1.0F / videoWidth * videoHeight / viewHeight;
                originScaleX = 1.0F;
            }
        }
    }
    setIdentityM(coordMatrix);
    rotateM(coordMatrix, 180, 0.0, 1.0, 0.0);
    rotateM(coordMatrix, 270.0, 0.0, 0.0, 1.0);
    scaleM(coordMatrix, 0, originScaleX, originScaleY, 1.0);
}
