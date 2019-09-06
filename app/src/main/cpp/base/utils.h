//
// Created by templechen on 2019-09-04.
//

#ifndef HYPLAYER_UTILS_H
#define HYPLAYER_UTILS_H

extern "C" {
#include <libavutil/rational.h>
};

long javaTimeMillis();

double r2d(AVRational r);

#endif //HYPLAYER_UTILS_H
