//
// Created by templechen on 2019-10-18.
//

#ifndef HYPLAYER_VIDEOPLAYER_H
#define HYPLAYER_VIDEOPLAYER_H


#include <demux/circle_av_packet_queue.h>
#include <demux/DemuxLooper.h>
#include <decode/DecodeLooper.h>
#include <android/native_window.h>
#include <video/GLVideoLooper.h>

class VideoPlayer {

public:

    VideoPlayer(const char* path, bool usingMediaCodec);

    virtual ~VideoPlayer();

    void start();

    void pause();

    void seek(long pos);

    void release();

    long getTotalDuration();

    long getCurrentDuration();

    void setNativeWindowCreated(ANativeWindow *nativeWindow);

    void setNativeWindowChanged(int width, int height);

    void setNativeWindowDestroyed();

    void doFrame();

private:

    const char *path;

    circle_av_packet_queue *packetQueue;
    circle_av_frame_queue *frameQueue;
    DemuxLooper *demuxLooper;
    DecodeLooper *decodeLooper;
    GLVideoLooper *glVideoLooper;
};


#endif //HYPLAYER_VIDEOPLAYER_H
