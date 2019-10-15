//
// Created by templechen on 2019-10-15.
//

#ifndef HYPLAYER_CAMERAFFVIDEOENCODER_H
#define HYPLAYER_CAMERAFFVIDEOENCODER_H


#include "../base/Looper.h"
#include "../encode/FFVideoEncoder.h"

class CameraFFVideoEncoder : public Looper{

public:

    enum {
        kMsgCameraEncodeStart,
        kMsgCameraEncoderFrame,
        kMsgCameraEncoderEnd
    };

    CameraFFVideoEncoder();

    ~CameraFFVideoEncoder();

    void handleMessage(LooperMessage *msg) override;

    void pthreadExit() override;

private:

    FFVideoEncoder *videoEncoder;

};


#endif //HYPLAYER_CAMERAFFVIDEOENCODER_H
