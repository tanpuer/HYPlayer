//
// Created by templechen on 2019-09-06.
//

#include <pthread.h>
#include "GifCreator.h"
#include "../base/native_log.h"
#include "../base/utils.h"

GifCreator::GifCreator(const char *path) {
    this->path = path;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&worker_thread, &attr, trampoline, this);
}

GifCreator::~GifCreator() {

}

void *GifCreator::trampoline(void *p) {
    ((GifCreator *) p)->startDecode();
    return nullptr;
}

AVFrame *GifCreator::readFrame(int index) {
    double currentTime = index * 16.6667;
    auto currentIndex = (unsigned int) (currentTime / (totalMs / size)) % size;
    if (currentIndex < frameList.size()) {
        ALOGD("currentIndex %d", currentIndex);
        return frameList.at(currentIndex);
    }
    return nullptr;
}

void GifCreator::startDecode() {
    long start = javaTimeMillis();
    ic = avformat_alloc_context();
    int re = avformat_open_input(&ic, path, nullptr, nullptr);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        ALOGE("open gif failed %s", buf);
        return;
    }
    ALOGD("open gif success");
    if (ic->nb_streams < 1) {
        ALOGE("no gif stream %d", ic->nb_streams);
        return;
    }
    AVCodecParameters *codecParameters = ic->streams[0]->codecpar;
    if (ic->streams[0]->duration > 0) {
        totalMs = ic->streams[0]->duration * r2d(ic->streams[0]->time_base) * 1000;
        size = (int) ic->streams[0]->nb_frames;
        ALOGD("gif total duration is %lld %d", totalMs, size);
    }
    AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
    if (codec == nullptr) {
        ALOGE("no codec for gif!");
        return;
    }
    ALOGD("gif find codec success");
    codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, codecParameters);
    re = avcodec_open2(codecContext, codec, nullptr);
    if (re != 0) {
        ALOGE("gif codec open error %s", path);
        return;
    }
    ALOGD("gif codec open success");

    out_buffer = (unsigned char *) av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height,
                                     1));
    img_convert_ctx = sws_getContext(codecContext->width, codecContext->height,
                                     codecContext->pix_fmt,
                                     codecContext->width, codecContext->height,
                                     AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    int count = 0;
    pkt = av_packet_alloc();
    frame = av_frame_alloc();
    while (av_read_frame(ic, pkt) == 0) {
        re = avcodec_send_packet(codecContext, pkt);
        if (re != 0) {
            ALOGE("gif send packet fail");
            return;
        }
        re = avcodec_receive_frame(codecContext, frame);
        if (re != 0) {
            ALOGE("gif receive frame fail");
            return;
        }
//        ALOGD("gif receive frame success");
        AVFrame *pFrameYUV = av_frame_alloc();
        out_buffer = (unsigned char *) av_malloc(
                av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecContext->width,
                                         codecContext->height,
                                         1));
        av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
                             AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height, 1);
        int height = sws_scale(img_convert_ctx, (const unsigned char *const *) frame->data,
                               frame->linesize, 0, codecContext->height,
                               pFrameYUV->data, pFrameYUV->linesize);
        if (height < 0) {
            ALOGE("sw_scale error!");
            return;
        }
        pFrameYUV->width = codecContext->width;
        pFrameYUV->height = codecContext->height;
        frameList.push_back(pFrameYUV);
        ALOGD("sw_scale success! %d", count);
        count++;
    }
    ALOGD("read all images in gif %ld %d", javaTimeMillis() - start, count);
}

void GifCreator::releaseFrame() {
    avcodec_close(codecContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&ic);
    av_packet_free(&pkt);
    av_frame_free(&frame);
    sws_freeContext(img_convert_ctx);
    if (out_buffer != nullptr) {
        av_free(out_buffer);
    }
    for (int i = 0; i < frameList.size(); ++i) {
//        frameList.pop_back()
    }
    ALOGD("GifCreator release");
}
