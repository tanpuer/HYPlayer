//
// Created by templechen on 2019-09-02.
//

#include "ImageCreator.h"
#include "../base/native_log.h"
#include "../base/utils.h"

AVFrame *ImageCreator::readImage(const char *path) {
    if (pFrameYUV != nullptr) {
        return pFrameYUV;
    }
    long start = javaTimeMillis();
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
    codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, codecParameters);
    re = avcodec_open2(codecContext, codec, nullptr);
    if (re != 0) {
        ALOGE("image codec open error %s", path);
        return nullptr;
    }
    ALOGD("image codec open success");

    pkt = av_packet_alloc();
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

    pFrameYUV = av_frame_alloc();
    out_buffer = (unsigned char *) av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height,
                                     1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
                         AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height, 1);
    img_convert_ctx = sws_getContext(codecContext->width, codecContext->height,
                                                 codecContext->pix_fmt,
                                                 codecContext->width, codecContext->height,
                                                 AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    int height = sws_scale(img_convert_ctx, (const unsigned char *const *) frame->data,
                           frame->linesize, 0, codecContext->height,
                           pFrameYUV->data, pFrameYUV->linesize);
    if (height < 0) {
        ALOGE("sw_scale error!");
        return nullptr;
    }
    pFrameYUV->width = codecContext->width;
    pFrameYUV->height = codecContext->height;

    long end = javaTimeMillis();
    ALOGD("read image time %ld", end - start);
    return pFrameYUV;
}

void ImageCreator::releaseImage() {
    avcodec_close(codecContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&ic);
    av_frame_free(&pFrameYUV);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    sws_freeContext(img_convert_ctx);
    if (out_buffer != nullptr) {
        av_free(out_buffer);
    }
    ALOGD("imageCreator release");
}

ImageCreator::~ImageCreator() {
    ALOGD("imageCreator delete success");
}
