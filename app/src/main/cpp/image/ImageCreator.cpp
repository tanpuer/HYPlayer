//
// Created by templechen on 2019-09-02.
//

#include "ImageCreator.h"
#include "../base/native_log.h"

extern "C" {
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
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
        avformat_close_input(&ic);
        ALOGE("open image failed %s", buf);

        return nullptr;
    }
    ALOGD("open image success");
    if (ic->nb_streams < 1) {
        ALOGE("no image stream %d", ic->nb_streams);
        avformat_close_input(&ic);
        return nullptr;
    }
    AVCodecParameters *codecParameters = ic->streams[0]->codecpar;
    AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
    if (codec == nullptr) {
        ALOGE("no codec for image!");
        avformat_close_input(&ic);
        return nullptr;
    }
    ALOGD("image find codec success")
    AVCodecContext *codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, codecParameters);
    re = avcodec_open2(codecContext, codec, nullptr);
    if (re != 0) {
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        avformat_close_input(&ic);
        ALOGE("image codec open error %s", path);
        return nullptr;
    }
    ALOGD("image codec open success");

    AVPacket *pkt = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    re = av_read_frame(ic, pkt);
    if (re != 0) {
        ALOGE("image read frame fail");
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        avformat_close_input(&ic);
        av_frame_free(&frame);
        return nullptr;
    }
    re = avcodec_send_packet(codecContext, pkt);
    if (re != 0) {
        ALOGE("image send packet fail");
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        avformat_close_input(&ic);
        av_frame_free(&frame);
        return nullptr;
    }
    while (true) {
        re = avcodec_receive_frame(codecContext, frame);
        if (re == 0) {
            break;
        }
    }
    ALOGD("image receive frame success");

    AVFrame *pFrameYUV = av_frame_alloc();
    unsigned char *out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height, 1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
                         AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height, 1);
    SwsContext *img_convert_ctx = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
                                     codecContext->width, codecContext->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    int height = sws_scale(img_convert_ctx, (const unsigned char* const*)frame->data, frame->linesize, 0, codecContext->height,
              pFrameYUV->data, pFrameYUV->linesize);
    if (height < 0) {
        ALOGE("sw_scale error!");
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        avformat_close_input(&ic);
        av_frame_free(&frame);
        return nullptr;
    }
    pFrameYUV->width = codecContext->width;
    pFrameYUV->height = codecContext->height;

    avcodec_close(codecContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&ic);
    av_frame_free(&frame);

    this->frame = pFrameYUV;
    return pFrameYUV;
}

void ImageCreator::releaseImage() {
    av_frame_free(&frame);
}
