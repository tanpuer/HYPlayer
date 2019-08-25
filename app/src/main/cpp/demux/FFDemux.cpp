//
// Created by templechen on 2019-08-18.
//

#include <pthread.h>
#include <unistd.h>
#include "FFDemux.h"
#include "../base/native_log.h"
#include "../base/AVPacketData.h"

extern "C" {
#include <libavformat/avformat.h>
}

//分数转为浮点数
static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0. : (double) r.num / (double) r.den;
}

bool FFDemux::init(const char *url) {
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
    ALOGD("open file begin %s", url)
    int re = avformat_open_input(&ic, url, nullptr, nullptr);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        ALOGD("open file failed %s", buf);
        return false;
    }
    ALOGD("open file success!")
    re = avformat_find_stream_info(ic, nullptr);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        ALOGD("find stream info failed %s", buf);
        return false;
    }
    totalDuration = ic->duration / AV_TIME_BASE;
    re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (re < 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        ALOGD("av_find_best_stream failed %s", buf);
        return false;
    }
    audioStreamIndex = re;
    return true;
}

bool FFDemux::start() {
    while (isDemuxing) {
        AVPacket *pkt = av_packet_alloc();
        int re = av_read_frame(ic, pkt);
        if (re != 0) {
            av_packet_free(&pkt);
            break;
        }
        if (pkt->stream_index == audioStreamIndex) {
            AVPacketData *data = new AVPacketData();
            data->packet = pkt;
            data->size = pkt->size;
            data->pts = pkt->pts * 1000 * r2d(ic->streams[pkt->stream_index]->time_base);
            data->parameters = ic->streams[audioStreamIndex]->codecpar;
            packetQueue->push(data);
            //network slow test
//            usleep(100000);
        } else {
            //video ignore
            av_packet_free(&pkt);
        }
    }
    //over
    AVPacketData *data = new AVPacketData();
    data->over = true;
    packetQueue->push(data);
    AVPacketData *data2 = new AVPacketData();
    data->over = true;
    packetQueue->push(data2);
    ALOGD("FFDemux start break!");
    return false;
}

bool FFDemux::pause() {
    return true;
}

bool FFDemux::seek(long pos) {
    return true;
}

void FFDemux::release() {
    if (ic) {
        avformat_close_input(&ic);
    }
    ALOGD("FFDemux release!");

    delete this;
}

FFDemux::~FFDemux() {
    ALOGD("delete FFDemux");
}
