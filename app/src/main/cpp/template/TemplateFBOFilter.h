//
// Created by templechen on 2019-09-16.
//

#ifndef HYPLAYER_TEMPLATEFBOFILTER_H
#define HYPLAYER_TEMPLATEFBOFILTER_H


#include "TemplateBaseFilter.h"

class TemplateFBOFilter : public TemplateBaseFilter {

public:

    TemplateFBOFilter();

    ~TemplateFBOFilter();

    void doFrame() override;

    void setNativeWindowSize(int width, int height) override;

    void release() override;

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

};


#endif //HYPLAYER_TEMPLATEFBOFILTER_H
