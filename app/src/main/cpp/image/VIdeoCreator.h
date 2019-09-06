//
// Created by templechen on 2019-09-06.
//

#ifndef HYPLAYER_VIDEOCREATOR_H
#define HYPLAYER_VIDEOCREATOR_H


#include "IAVFrameCreator.h"

class VIdeoCreator : public IAVFrameCreator {

public:

    virtual AVFrame *readFrame(const char *path, int index = 0);

    virtual void releaseFrame();
};


#endif //HYPLAYER_VIDEOCREATOR_H
