//
// Created by templechen on 2019-09-06.
//

#include <pthread.h>
#include "VideoCreator.h"
#include "../base/utils.h"
#include "../base/native_log.h"

VideoCreator::VideoCreator(const char *path) {
    this->path = path;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&worker_thread, &attr, trampoline, this);
}

VideoCreator::~VideoCreator() {

}

AVFrame *VideoCreator::readFrame(int index) {
    return nullptr;
}

void VideoCreator::releaseFrame() {
    avcodec_close(codecContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&ic);
    av_packet_free(&pkt);
    for (unsigned int i = 0; i < frameList.size(); ++i) {
        av_frame_free(&frameList.at(i));
    }
    frameList.clear();
    ALOGD("GifCreator release");
}

void *VideoCreator::trampoline(void *p) {
    ((VideoCreator *) p)->startEncode();
    return nullptr;
}

void VideoCreator::startEncode() {
    long start = javaTimeMillis();
    ic = avformat_alloc_context();
    int re = avformat_open_input(&ic, path, nullptr, nullptr);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        ALOGE("open video failed %s", buf);
        return;
    }
    ALOGD("open video success");
    if (ic->nb_streams < 1) {
        ALOGE("no video stream %d", ic->nb_streams);
        return;
    }
    AVCodecParameters *codecParameters = ic->streams[0]->codecpar;
    if (ic->streams[0]->duration > 0) {
        totalMs = ic->streams[0]->duration * r2d(ic->streams[0]->time_base) * 1000;
        size = (int) ic->streams[0]->nb_frames;
        ALOGD("video total duration is %lld %d", totalMs, size);
    }
    AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
    if (codec == nullptr) {
        ALOGE("no codec for video!");
        return;
    }
    ALOGD("video find codec success");
    codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, codecParameters);
    re = avcodec_open2(codecContext, codec, nullptr);
    if (re != 0) {
        ALOGE("video codec open error %s", path);
        return;
    }
    ALOGD("video codec open success");
    int count = 0;
    pkt = av_packet_alloc();
    while (av_read_frame(ic, pkt) == 0) {
        re = avcodec_send_packet(codecContext, pkt);
        if (re != 0) {
            ALOGE("video send packet fail");
            return;
        }
        AVFrame *pFrameYUV = av_frame_alloc();
        re = avcodec_receive_frame(codecContext, pFrameYUV);
        if (re != 0) {
            ALOGE("video receive frame fail");
            return;
        }
        pFrameYUV->width = codecContext->width;
        pFrameYUV->height = codecContext->height;
        frameList.push_back(pFrameYUV);
        count++;
    }
    ALOGD("read all images in gif %ld %d", javaTimeMillis() - start, count);
}
