//
// Created by templechen on 2019-10-17.
//

#ifndef FFMPEG_PLAYER_MEDIACODEC_NV12_FILTER_H
#define FFMPEG_PLAYER_MEDIACODEC_NV12_FILTER_H


#include "base_filter.h"

class mediacodec_nv12_filter : public base_filter{

public:

protected:

    void initFragmentShader() override;

    void createTextures() override;

    void releaseTextures() override;

    void drawTextures(AVFrame *avFrame) override;

    GLuint uvTexture;
    GLint uTextureUVLocation = -1.0;
    const GLchar *uTextureUV = "uTextureUV";
};


#endif //FFMPEG_PLAYER_MEDIACODEC_NV12_FILTER_H
