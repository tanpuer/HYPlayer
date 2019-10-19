//
// Created by templechen on 2019-10-17.
//

#ifndef FFMPEG_PLAYER_MEDIACODEC_NV21_FILTER_H
#define FFMPEG_PLAYER_MEDIACODEC_NV21_FILTER_H


#include "mediacodec_nv12_filter.h"

class mediacodec_nv21_filter : public mediacodec_nv12_filter{\

protected:
    void initFragmentShader() override;

};


#endif //FFMPEG_PLAYER_MEDIACODEC_NV21_FILTER_H
