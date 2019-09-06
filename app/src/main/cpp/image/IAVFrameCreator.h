//
// Created by templechen on 2019-09-06.
//

#ifndef HYPLAYER_IAVFRAMECREATOR_H
#define HYPLAYER_IAVFRAMECREATOR_H


extern "C" {
#include <libavutil/frame.h>
};

class IAVFrameCreator {

public:

    virtual AVFrame *readFrame(const char *path) = 0;

    virtual void releaseFrame() = 0;

};


#endif //HYPLAYER_IAVFRAMECREATOR_H
