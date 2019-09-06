//
// Created by templechen on 2019-09-06.
//

#ifndef HYPLAYER_GIFCREATOR_H
#define HYPLAYER_GIFCREATOR_H


#include "IAVFrameCreator.h"

class GifCreator : public IAVFrameCreator {

public:

    virtual AVFrame *readFrame(const char *path, int index = 0);

    virtual void releaseFrame();

    ~GifCreator();

};


#endif //HYPLAYER_GIFCREATOR_H
