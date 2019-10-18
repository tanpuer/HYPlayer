//
// Created by templechen on 2019-10-18.
//

#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(const char *path) {
    packetQueue = new circle_av_packet_queue();
    frameQueue = new circle_av_frame_queue();
    demuxLooper = new DemuxLooper(packetQueue);
    decodeLooper = new DecodeLooper(frameQueue, packetQueue);
    demuxLooper->sendMessage(demuxLooper->kMsgDemuxCreated, (void *) path);
    decodeLooper->sendMessage(decodeLooper->kMsgDecodeCreated);
}

VideoPlayer::~VideoPlayer() {

}

void VideoPlayer::start() {
//    if (slPlayerLooper != nullptr) {
//        slPlayerLooper->sendMessage(slPlayerLooper->kMsgSLPlayerStart);
//    }
}

void VideoPlayer::pause() {
//    if (slPlayerLooper != nullptr) {
//        slPlayerLooper->sendMessage(slPlayerLooper->kMsgSLPlayerPause);
//    }
}

void VideoPlayer::seek(long pos) {
//    if (slPlayerLooper != nullptr) {
//        slPlayerLooper->sendMessage(slPlayerLooper->kMsgSLPlayerSeek);
//    }
    if (demuxLooper != nullptr) {
        demuxLooper->demux->isDemuxing = false;
        demuxLooper->sendMessage(demuxLooper->kMsgDemuxSeek, reinterpret_cast<void *>(pos));
    }
}

void VideoPlayer::release() {
//    if (slPlayerLooper != nullptr) {
//        slPlayerLooper->sendMessage(slPlayerLooper->kMsgSLPlayerRelease);
//    }
    if (demuxLooper != nullptr) {
        demuxLooper->demux->isDemuxing = false;
        demuxLooper->demux->isOver = true;
        demuxLooper->sendMessage(demuxLooper->kMsgDemuxRelease);
    }
    if (decodeLooper != nullptr) {
        decodeLooper->sendMessage(decodeLooper->kMsgDecodeRelease);
    }
    delete this;
}

// thread unsafe
long VideoPlayer::getTotalDuration() {
    if (demuxLooper != nullptr) {
        return demuxLooper->getTotalDuration();
    }
    return 0;
}

//thread unsafe
long VideoPlayer::getCurrentDuration() {
//    if (slPlayerLooper != nullptr) {
//        return slPlayerLooper->getCurrentPos();
//    }
    return 0;
}

void VideoPlayer::setNativeWindowCreated(ANativeWindow *nativeWindow) {

}

void VideoPlayer::setNativeWindowDestroyed() {

}

void VideoPlayer::setNativeWindowChanged(int width, int height) {

}

void VideoPlayer::doFrame() {

}
