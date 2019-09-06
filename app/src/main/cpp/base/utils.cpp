//
// Created by templechen on 2019-09-04.
//

#include <sys/time.h>
#include "utils.h"

long javaTimeMillis() {
    timeval time;
    gettimeofday(&time, nullptr);
    return long(time.tv_sec) * 1000 + long(time.tv_usec / 1000);
}

double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0. : (double) r.num / (double) r.den;
}