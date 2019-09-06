//
// Created by templechen on 2019-09-06.
//

#include "GifCreator.h"
#include "../base/native_log.h"
#include "../base/utils.h"

AVFrame *GifCreator::readFrame(const char *path, int index) {
    long start = javaTimeMillis();

    int re = avformat_open_input(&ic, path, nullptr, nullptr);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        ALOGE("open gif failed %s", buf);
        return nullptr;
    }
    ALOGD("open gif success");
    if (ic->nb_streams < 1) {
        ALOGE("no gif stream %d", ic->nb_streams);
        return nullptr;
    }
    AVCodecParameters *codecParameters = ic->streams[0]->codecpar;
    AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
    if (codec == nullptr) {
        ALOGE("no codec for gif!");
        return nullptr;
    }
    ALOGD("gif find codec success");
    codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, codecParameters);
    re = avcodec_open2(codecContext, codec, nullptr);
    if (re != 0) {
        ALOGE("gif codec open error %s", path);
        return nullptr;
    }
    ALOGD("gif codec open success");

    pkt = av_packet_alloc();
    while (av_read_frame(ic, pkt) == 0) {
        re = avcodec_send_packet(codecContext, pkt);
        if (re != 0) {
            ALOGE("gif send packet fail");
            return nullptr;
        }
        AVFrame *frame = av_frame_alloc();
        re = avcodec_receive_frame(codecContext, frame);
        if (re != 0) {
            ALOGE("gif receive frame fail");
            return nullptr;
        }
        ALOGD("gif receive frame success");
    }

    ALOGD("read all images in gif %ld", javaTimeMillis() - start);
    return nullptr;
}

void GifCreator::releaseFrame() {

}

GifCreator::~GifCreator() {

}
