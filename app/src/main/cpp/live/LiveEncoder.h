//
// Created by templechen on 2019-09-27.
//

#ifndef HYPLAYER_LIVEENCODER_H
#define HYPLAYER_LIVEENCODER_H


#include <encode/FFVideoEncoder.h>
#include <jni.h>
#include "../base/Looper.h"

struct CameraNV21Data {
    unsigned char* data;
    int width;
    int height;
    long long pts;
};

class LiveEncoder : public Looper{

public:

    enum {
        kLiveEncoderStart,
        kLiveEncoderDoFrame,
        kLiveEncoderStop
    };

    LiveEncoder(JavaVM *vm);

    ~LiveEncoder();

    void handleMessage(LooperMessage *msg) override;

    void pthreadExit() override;

private:

    FFVideoEncoder *encoder;

    JavaVM *vm;

    JNIEnv *env = nullptr;

};


#endif //HYPLAYER_LIVEENCODER_H
