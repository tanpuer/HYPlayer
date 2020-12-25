//
// Created by templechen on 2019-09-16.
//

#include "TemplateFBOFilter.h"
#include <skia/gpu/gl/GrGLInterface.h>
#include <skia/core/SkPaint.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkGraphics.h>
#include <skia/gpu/GrBackendSurface.h>
#include <skia/effects/SkDiscretePathEffect.h>
#include <skia/gpu/gl/GrGLDefines.h>
#include <skia/effects/Sk2DPathEffect.h>
#include <base/utils.h>
#include "../base/gl_utils.h"

#define GET_STR(x) #x

static const char *VERTEX_SHADER_STR = GET_STR(
        attribute vec4 fboPosition;
        uniform mat4 fboTextureMatrix;
        attribute vec4 fboTextureCoordinate;
        varying vec2 fboTextureCoord;
        void main() {
            fboTextureCoord = (fboTextureMatrix * fboTextureCoordinate).xy;
            gl_Position = fboPosition;
        }
);

static const char *FRAGMENT_SHADER_STR = GET_STR(
        precision
        highp float;
        varying
        vec2 fboTextureCoord;
        uniform
        sampler2D fboTexture;
        void main() {
            gl_FragColor = texture2D(fboTexture, fboTextureCoord);
        }
);

TemplateFBOFilter::TemplateFBOFilter() {
    fboVertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_STR);
    fboFragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_STR);
    fboProgram = createShaderProgram(fboVertexShader, fboFragmentShader);
    ALOGD("fbo %d %d %d", fboProgram, fboFragmentShader, fboVertexShader);
    fboTexMatrix = ESMatrix();
    setIdentityM(&fboTexMatrix);

    paint = new TestPaint();
}

TemplateFBOFilter::~TemplateFBOFilter() {
    delete paint;
}

void TemplateFBOFilter::doFrame() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           frameBufferTextureId, 0);

    TemplateBaseFilter::doFrame();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //fbo
    glUseProgram(fboProgram);

    GLint vertexCount = sizeof(vertex) / (sizeof(vertex[0]) * 2);
    fboPositionLocation = glGetAttribLocation(fboProgram, fboPosition);
    glEnableVertexAttribArray(fboPositionLocation);
    glVertexAttribPointer(fboPositionLocation, 2, GL_FLOAT, GL_FALSE, 8, vertex);

    fboTextureCoordinateLocation = glGetAttribLocation(fboProgram, fboTextureCoordinate);
    glEnableVertexAttribArray(fboTextureCoordinateLocation);
    glVertexAttribPointer(fboTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 8, texture);

    fboTextureMatrixLocation = glGetUniformLocation(fboProgram, fboTextureMatrix);
    glUniformMatrix4fv(fboTextureMatrixLocation, 1, GL_FALSE, fboTexMatrix.m);

    fboTextureLocation = glGetUniformLocation(fboProgram, fboTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frameBufferTextureId);
    glUniform1i(fboTextureLocation, 0);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    long start = javaTimeMillis();
    if (skia_surface == nullptr) {
        SkGraphics::Init();
        sk_sp<const GrGLInterface> interface(GrGLMakeNativeInterface());
        context = GrContext::MakeGL(interface);
        SkASSERT(context);
        // Wrap the frame buffer object attached to the screen in a Skia render target so Skia can
        // render to it
        GrGLint buffer = 0;
        GrGLFramebufferInfo info;
        info.fFBOID = (GrGLuint) buffer;
        SkColorType colorType;
        info.fFormat = GR_GL_RGBA8;
        colorType = kRGBA_8888_SkColorType;
        GrBackendRenderTarget target(windowWidth, windowHeight, 0, 8, info);

        // setup SkSurface
        // To use distance field text, use commented out SkSurfaceProps instead
        // SkSurfaceProps props(SkSurfaceProps::kUseDeviceIndependentFonts_Flag,
        //                      SkSurfaceProps::kLegacyFontHost_InitType);
        SkSurfaceProps props(SkSurfaceProps::kLegacyFontHost_InitType);
        skia_surface = (SkSurface::MakeFromBackendRenderTarget(context.get(), target,
                                                               reverse ? kBottomLeft_GrSurfaceOrigin
                                                                       : kTopLeft_GrSurfaceOrigin,
                                                               colorType, nullptr, &props));
        SkASSERT(skia_surface);
    }
    SkCanvas *canvas = skia_surface->getCanvas();
    paint->onDraw(canvas, windowWidth, windowHeight);
    canvas->flush();
//    ALOGD("skia draw time %ld", javaTimeMillis() - start);

    glDisableVertexAttribArray(fboPositionLocation);
    glDisableVertexAttribArray(fboTextureCoordinateLocation);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TemplateFBOFilter::genFrameBuffer(int width, int height) {
    glGenFramebuffers(1, &frameBuffer);
    frameBufferTextureId = createTexture(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, frameBufferTextureId);
    glViewport(0, 0, width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ALOGE("fbo glFramebufferTexture2D error");
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ALOGD("fbo genFrameBuffer %d %d", frameBuffer, frameBufferTextureId);
}

void TemplateFBOFilter::setNativeWindowSize(int width, int height) {
    TemplateBaseFilter::setNativeWindowSize(width, height);
    genFrameBuffer(width, height);
}

void TemplateFBOFilter::release() {
    TemplateBaseFilter::release();

    ALOGD("TemplateFBOFilter release");
    glDeleteProgram(fboProgram);
    glDeleteShader(fboFragmentShader);
    glDeleteShader(fboVertexShader);
    auto textures = new GLuint[1];
    textures[0] = frameBufferTextureId;
    glDeleteTextures(3, textures);
    auto frameBuffers = new GLuint[1];
    frameBuffers[0] = frameBuffer;
    glDeleteFramebuffers(1, frameBuffers);
}
