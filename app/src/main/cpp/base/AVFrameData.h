//
// Created by templechen on 2019-08-19.
//

#ifndef HYPLAYER_AVFRAMEDATA_H
#define HYPLAYER_AVFRAMEDATA_H


extern "C" {
#include <libavutil/frame.h>
};

class AVFrameData {

public:

    AVFrame *frame;

    int64_t pts;

    unsigned char* data;

    void clear();

    int size;

    bool over;

};


#endif //HYPLAYER_AVFRAMEDATA_H
