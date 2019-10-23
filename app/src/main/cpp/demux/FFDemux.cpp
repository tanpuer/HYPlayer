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
    totalDuration = ic->duration / AV_TIME_BASE * 1000;
    re = getBestStream(ic);
    if (re < 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        ALOGD("av_find_best_stream failed %s", buf);
        return false;
    }
    streamIndex = re;
    timeBase = r2d(ic->streams[streamIndex]->time_base);
    ALOGD("FFDemux init finish");
    return true;
}

bool FFDemux::start() {
    ALOGD("FFDemux start");
    while (isDemuxing) {
        AVPacket *pkt = av_packet_alloc();
        int re = av_read_frame(ic, pkt);
        if (re != 0) {
            char buf[1024] = {0};
            av_strerror(re, buf, sizeof(buf));
            ALOGD("av_read_frame error %s", buf);
            av_packet_free(&pkt);
            if (!isOver && loop) {
                seekToStart();
                continue;
            }
            break;
        }
        if (pkt->stream_index == streamIndex) {
            AVPacketData *data = new AVPacketData();
            data->packet = pkt;
            data->size = pkt->size;
            data->pts = pkt->pts * 1000 * timeBase;
            data->parameters = ic->streams[streamIndex]->codecpar;
            data->timeBase = timeBase;
            packetQueue->push(data);
            //network slow test
//            usleep(100000);
        } else {
            //video ignore
            av_packet_free(&pkt);
        }
    }
    if (isOver) {
        //over
        AVPacketData *data = new AVPacketData();
        data->over = true;
        packetQueue->push(data);
        AVPacketData *data2 = new AVPacketData();
        data->over = true;
        packetQueue->push(data2);
        isOver = false;
    }
    if (needReset) {
        needReset = false;
        auto *data = new AVPacketData();
        data->reset = true;
        packetQueue->push(data);
        needReset = false;
        if (ic) {
            avformat_close_input(&ic);
        }
        ALOGD("FFDemux reset!");
    }
    ALOGD("FFDemux start break!");
    return false;
}

bool FFDemux::pause() {
    return true;
}

bool FFDemux::seek(long pos) {
    ALOGD("demux seek start! %ld, %lld", pos, totalDuration);
    avformat_flush(ic);
    int64_t seekPos = ic->streams[streamIndex]->duration * pos / totalDuration;
    ALOGD("demux seek start! %lld, %lld", seekPos, ic->streams[streamIndex]->duration);
    int re = av_seek_frame(ic, streamIndex, seekPos, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);
    if (re < 0) {
        ALOGE("seek error !");
        return false;
    }
    isDemuxing = true;
    AVPacketData *data = new AVPacketData();
    data->seekOver = true;
    packetQueue->push(data);
    ALOGD("demux seek end!")
    start();
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

int FFDemux::getBestStream(AVFormatContext *ic) {
    return av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
}

void FFDemux::seekToStart() {
    avformat_flush(ic);
    ALOGD("demux seek start! %d, %lld", 0, ic->streams[streamIndex]->duration);
    int re = av_seek_frame(ic, streamIndex, 0, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);
    if (re < 0) {
        ALOGE("seek error !");
    }
}
