//
// Created by templechen on 2019-11-25.
//

#ifndef HYPLAYER_SLRECODERLOOPER_H
#define HYPLAYER_SLRECODERLOOPER_H


#include "../base/Looper.h"
#include "SLAudioRecoder.h"

class SLRecoderLooper : public Looper {

public:

    enum {
        kMsgSLRecoderCreated,
        kMsgSLRecoderStart,
        kMsgSLRecoderPause,
        kMsgSLRecoderRelease
    };

    void handleMessage(LooperMessage *msg) override;

    void pthreadExit() override;

private:

    SLAudioRecoder *audioRecoder = nullptr;

};


#endif //HYPLAYER_SLRECODERLOOPER_H
