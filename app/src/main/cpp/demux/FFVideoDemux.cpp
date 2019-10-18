//
// Created by templechen on 2019-10-18.
//

#include "FFVideoDemux.h"

int FFVideoDemux::getBestStream(AVFormatContext *ic) {
    return av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
}
