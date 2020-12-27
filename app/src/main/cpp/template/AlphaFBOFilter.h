//
// Created by cw on 2020/12/25.
//

#ifndef HYPLAYER_ALPHAFBOFILTER_H
#define HYPLAYER_ALPHAFBOFILTER_H


#include "TemplateBaseFilter.h"

class AlphaFBOFilter : public TemplateBaseFilter {

public:

    AlphaFBOFilter();

    ~AlphaFBOFilter();

    void doFrame() override;

    void setNativeWindowSize(int width, int height) override;

    void release() override;

    bool reverse = true;

private:

    void genFrameBuffer(int width, int height);

    GLuint frameBufferTextureId = 0;
    GLuint frameBuffer = 0;
    GLuint fboVertexShader = 0;
    GLuint fboFragmentShader = 0;
    GLuint fboProgram = 0;

    const char *fboPosition = "fboPosition";
    const char *fboTextureMatrix = "fboTextureMatrix";
    const char *fboTextureCoordinate = "fboTextureCoordinate";
    const char *fboTexture = "fboTexture";

    GLint fboPositionLocation = 0;
    GLint fboTextureMatrixLocation = 0;
    GLint fboTextureCoordinateLocation = 0;
    GLint fboTextureLocation = 0;

    ESMatrix fboTexMatrix;

};


#endif //HYPLAYER_ALPHAFBOFILTER_H
