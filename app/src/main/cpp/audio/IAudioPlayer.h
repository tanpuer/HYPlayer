//
// Created by templechen on 2019-08-20.
//

#ifndef HYPLAYER_IAUDIOPLAYER_H
#define HYPLAYER_IAUDIOPLAYER_H


class IAudioPlayer {

public:

    virtual void init() = 0;

    virtual void start() = 0;

    virtual void pause() = 0;

    virtual void seek() = 0;

    virtual void release() = 0;

    virtual void reset() = 0;

    virtual void setVolume(float volume) = 0;

    long currentPos;

};


#endif //HYPLAYER_IAUDIOPLAYER_H
