//
// Created by templechen on 2019-11-25.
//

#ifndef HYPLAYER_SLAUDIORECODER_H
#define HYPLAYER_SLAUDIORECODER_H


class SLAudioRecoder {

public:

    SLAudioRecoder(const char *path);

    void start();

    void pause();

    void release();

    void call(void *bufq);

private:

    const char *path;

    short *mBuffers[2];

    int mIndex = 0;

    const short BUFFER_SIZE = 2048;
};


#endif //HYPLAYER_SLAUDIORECODER_H
