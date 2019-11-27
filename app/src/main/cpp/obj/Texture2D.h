//
// Created by templechen on 2019-11-27.
//

#ifndef HYPLAYER_TEXTURE2D_H
#define HYPLAYER_TEXTURE2D_H


#include <GLES2/gl2.h>
#include <image/ImageCreator.h>

class Texture2D {

public:

    Texture2D(const char* path);

    ~Texture2D();

    void create();

    void bindTexture();

private:

    const char* path;

    GLuint texId = GL_INVALID_VALUE;

    ImageCreator *imageCreator;
};


#endif //HYPLAYER_TEXTURE2D_H
