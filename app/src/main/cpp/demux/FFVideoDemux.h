//
// Created by templechen on 2019-10-18.
//

#ifndef HYPLAYER_FFVIDEODEMUX_H
#define HYPLAYER_FFVIDEODEMUX_H


#include "FFDemux.h"
#include "../../jniLibs/includes/libavformat/avformat.h"

class FFVideoDemux : public FFDemux{
    
protected:
    virtual int getBestStream(AVFormatContext *ic);
};


#endif //HYPLAYER_FFVIDEODEMUX_H
