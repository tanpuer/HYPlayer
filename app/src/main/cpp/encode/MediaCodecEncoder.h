//
// Created by templechen on 2019-09-05.
//

#ifndef HYPLAYER_MEDIACODECENCODER_H
#define HYPLAYER_MEDIACODECENCODER_H


#include <jni.h>
#include <pthread.h>

class MediaCodecEncoder {

public:

    MediaCodecEncoder(jobject surface, jobject javaMediaEncoder, JavaVM *vm);

    ~MediaCodecEncoder();

private:

    jobject surface;
    jobject javaMediaEncoder;
    JavaVM *vm;

    pthread_t worker_thread;

    static void *trampoline(void *p);

};


#endif //HYPLAYER_MEDIACODECENCODER_H
