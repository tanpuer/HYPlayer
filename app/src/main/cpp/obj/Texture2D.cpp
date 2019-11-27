//
// Created by templechen on 2019-11-27.
//

#include <assert.h>
#include <base/native_log.h>
#include "Texture2D.h"

Texture2D::Texture2D(const char *path) {
    this->path = path;
}

void Texture2D::create() {
    glGenTextures(1, &texId);
    assert(texId != GL_INVALID_VALUE);
    glBindTexture(GL_TEXTURE_2D, texId);
    glActiveTexture(GL_TEXTURE0);

    imageCreator = new ImageCreator(path, AV_PIX_FMT_RGBA);
    AVFrame *frame = imageCreator->readFrame(0);
    assert(frame != nullptr && frame->width > 0 && frame->height > 0);
    glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, frame->width, frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            frame->data[0]
    );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glActiveTexture(GL_TEXTURE0);
}

void Texture2D::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, texId);
    glActiveTexture(GL_TEXTURE0);
}

Texture2D::~Texture2D() {
    if (texId != GL_INVALID_VALUE) {
        glDeleteTextures(1, &texId);
        texId = GL_INVALID_VALUE;
    }
    if (imageCreator != nullptr) {
        imageCreator->releaseFrame();
        delete imageCreator;
        imageCreator = nullptr;
    }
}
