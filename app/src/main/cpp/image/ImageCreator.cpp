//
// Created by templechen on 2019-09-02.
//

#include "ImageCreator.h"
#include "../base/native_log.h"

extern "C" {
#include <libavformat/avformat.h>
}

AVFrame *ImageCreator::readImage(const char *path) {
    if (frame != nullptr) {
        return frame;
    }

    AVFormatContext *ic = nullptr;
    int re = avformat_open_input(&ic, path, nullptr, nullptr);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        ALOGE("open image failed %s", buf);
        return nullptr;
    }
    ALOGD("open image success");
    if (ic->nb_streams < 1) {
        ALOGE("no image stream %d", ic->nb_streams);
        return nullptr;
    }
    AVCodecParameters *codecParameters = ic->streams[0]->codecpar;
    AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
    if (codec == nullptr) {
        ALOGE("no codec for image!");
        return nullptr;
    }
    ALOGD("image find codec success")
    AVCodecContext *codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, codecParameters);
    re = avcodec_open2(codecContext, codec, nullptr);
    if (re != 0) {
        ALOGE("image codec open error %s", path);
        return nullptr;
    }
    ALOGD("image codec open success");

    AVPacket *pkt = av_packet_alloc();
    frame = av_frame_alloc();
    re = av_read_frame(ic, pkt);
    if (re != 0) {
        ALOGE("image read frame fail");
        return nullptr;
    }
    re = avcodec_send_packet(codecContext, pkt);
    if (re != 0) {
        ALOGE("image send packet fail");
        return nullptr;
    }
    while (true) {
        re = avcodec_receive_frame(codecContext, frame);
        if (re == 0) {
            break;
        }
    }
    ALOGD("image receive frame success");

    //todo 上面的return 也要处理下资源释放
    avcodec_close(codecContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&ic);
    return frame;
}

void ImageCreator::releaseImage(AVFrame *avFrame) {
    av_frame_free(&avFrame);
}
