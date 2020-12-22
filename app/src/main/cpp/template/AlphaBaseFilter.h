//
// Created by DZSB-000848 on 2020/9/19.
//

#ifndef HYPLAYER_ALPHABASEFILTER_H
#define HYPLAYER_ALPHABASEFILTER_H


#include "TemplateBaseFilter.h"

class AlphaBaseFilter : public TemplateBaseFilter{

public:

    AlphaBaseFilter();

    virtual ~AlphaBaseFilter();

    virtual void updateMatrix();

    void initGLProgram() override;

    void doFrame() override;

    void release() override;

private:

    void initTransPrograms();

    void releaseTransProgram();

    GLuint transVertexShader = 0;
    GLuint transFragmentShader = 0;
    GLuint transProgram = 0;

    GLuint aTransPositionLocation = -1;
};


#endif //HYPLAYER_ALPHABASEFILTER_H
