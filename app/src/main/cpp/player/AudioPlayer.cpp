//
// Created by templechen on 2019-08-20.
//

#include "AudioPlayer.h"
#include <string>

AudioPlayer::AudioPlayer(const char *path) {
    packetQueue = new circle_av_packet_queue();
    frameQueue = new circle_av_frame_queue();
    demuxLooper = new DemuxLooper(packetQueue);
    decodeLooper = new DecodeLooper(frameQueue, packetQueue, true, false);
    demuxLooper->sendMessage(demuxLooper->kMsgDemuxCreated, (void *) path);
    decodeLooper->sendMessage(decodeLooper->kMsgDecodeCreated);
    slPlayerLooper = new SLPlayerLooper(frameQueue);
    slPlayerLooper->sendMessage(slPlayerLooper->kMsgSLPlayerCreated);
}

AudioPlayer::~AudioPlayer() {

}

void AudioPlayer::start() {
    if (slPlayerLooper != nullptr) {
        slPlayerLooper->sendMessage(slPlayerLooper->kMsgSLPlayerStart);
    }
}

void AudioPlayer::pause() {
    if (slPlayerLooper != nullptr) {
        slPlayerLooper->sendMessage(slPlayerLooper->kMsgSLPlayerPause);
    }
}

void AudioPlayer::seek(long pos) {
    if (slPlayerLooper != nullptr) {
        slPlayerLooper->sendMessage(slPlayerLooper->kMsgSLPlayerSeek);
    }
    if (demuxLooper != nullptr) {
        demuxLooper->demux->isDemuxing = false;
        demuxLooper->sendMessage(demuxLooper->kMsgDemuxSeek, reinterpret_cast<void *>(pos));
    }
}

void AudioPlayer::release() {
    if (slPlayerLooper != nullptr) {
        slPlayerLooper->sendMessage(slPlayerLooper->kMsgSLPlayerRelease);
    }
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
long AudioPlayer::getTotalDuration() {
    if (demuxLooper != nullptr) {
        return demuxLooper->getTotalDuration();
    }
    return 0;
}

//thread unsafe
long AudioPlayer::getCurrentDuration() {
    if (slPlayerLooper != nullptr) {
        return slPlayerLooper->getCurrentPos();
    }
    return 0;
}

void AudioPlayer::setLoop(bool loop) {
    if (demuxLooper != nullptr) {
        demuxLooper->demux->isOver = false;
        demuxLooper->demux->loop = loop;
    }
}
