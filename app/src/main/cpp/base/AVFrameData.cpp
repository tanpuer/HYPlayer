//
// Created by templechen on 2019-08-19.
//

#include "AVFrameData.h"

void AVFrameData::clear() {
    av_frame_free(&frame);
    free(data);
    data = nullptr;
}
