//
// Created by templechen on 2019-08-09.
//

#ifndef HYPLAYER_AVPACKETDATA_H
#define HYPLAYER_AVPACKETDATA_H


#include <cstdint>

extern "C" {
#include <libavcodec/avcodec.h>
};

class AVPacketData {

public:

    int size;

    AVPacket *packet;

    int64_t pts;

    AVCodecParameters *parameters;

    bool over;

    void clear();

};


#endif //HYPLAYER_AVPACKETDATA_H
