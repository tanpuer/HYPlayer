//
// Created by templechen on 2019-11-27.
//

#include <assert.h>
#include <base/native_log.h>
#include "Texture2D.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "timerutil.h"

Texture2D::Texture2D(const char *path) {
    this->path = path;
}

void Texture2D::create() {
    glGenTextures(1, &texId);
    ALOGD("111111 create texture id is %d", texId);
    assert(texId != GL_INVALID_VALUE);
    glBindTexture(GL_TEXTURE_2D, texId);

//     tga/bmp files are saved as vertical mirror images ( at least more than half ).
    timerutil t;
    t.start();
    stbi_set_flip_vertically_on_load(1);
    int comp;
    image = stbi_load(path, &w, &h, &comp, STBI_default);
    t.end();
    ALOGD("Parsing time:%s %d %lu [msecs] %d %d %d\n", path, texId, t.msec(), w, h, comp);
    if (comp == 3) {
        format = GL_RGB;
    } else if (comp == 4) {
        format = GL_RGBA;
    } else {
        //todo
        ALOGE("unSupport type");
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bindTexture() {
    glActiveTexture(GL_TEXTURE0 + texId - 1);
    glBindTexture(GL_TEXTURE_2D, texId);
    if (!textured) {
        glTexImage2D(
                GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
                image
        );
        textured = true;
    }
}

Texture2D::~Texture2D() {
    if (texId != GL_INVALID_VALUE) {
        glDeleteTextures(1, &texId);
        texId = GL_INVALID_VALUE;
    }
    if (image != nullptr) {
        stbi_image_free(image);
    }
}
