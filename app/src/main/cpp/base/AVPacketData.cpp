//
// Created by templechen on 2019-08-09.
//

#include "AVPacketData.h"

void AVPacketData::clear() {
    av_packet_free(&packet);
}
