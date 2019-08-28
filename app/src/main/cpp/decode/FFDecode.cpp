//
// Created by templechen on 2019-08-19.
//

#include "FFDecode.h"
#include "../base/native_log.h"

bool FFDecode::init() {
    return true;
}

bool FFDecode::start() {
    while (isDecoding) {
        if (parameters == nullptr) {
            AVPacketData *packetData = packetQueue->pull();
            if (packetData == nullptr) {
                continue;
            }
            parameters = packetData->parameters;
            timeBase = packetData->timeBase;
            codec = avcodec_find_decoder(parameters->codec_id);
            if (codec == nullptr) {
                ALOGE("avcodec_find_decoder %d failed", parameters->codec_id);
                return false;
            }
            ALOGD("avcodec_find_decoder success");
            codecContext = avcodec_alloc_context3(codec);
            codecContext->thread_count = 1;
            avcodec_parameters_to_context(codecContext, parameters);
            int re = avcodec_open2(codecContext, codec, nullptr);
            if (re != 0) {
                char buf[1024] = {0};
                av_strerror(re, buf, sizeof(buf) - 1);
                ALOGE("avcodec open failed !%s", buf);
                return false;
            }
            swrContext = swr_alloc();
            swr_alloc_set_opts(swrContext,
                               av_get_default_channel_layout(2),
                               AV_SAMPLE_FMT_S16,
                               44100,
                               av_get_default_channel_layout(parameters->channels),
                               (AVSampleFormat) parameters->format,
                               parameters->sample_rate,
                               0,
                               0
            );
            re = swr_init(swrContext);
            if (re != 0) {
                ALOGE("swr_init failed!");
                return false;
            }
            ALOGD("swr_init success!");
        }
//        ALOGD("pull AVPacket data from packetQueue!");
        AVPacketData *packetData = packetQueue->pull();
        if (packetData == nullptr) {
            continue;
        }
        if (packetData->seekOver) {
            ALOGD("FFDecode receive seek over signal");
            auto *frameData = new AVFrameData();
            frameData->seekOver = true;
            frameQueue->push(frameData);
            continue;
        }
        if (packetData->over) {
            ALOGD("AVPacket is over!");
            packetData->clear();
            auto *frameData = new AVFrameData();
            frameData->over = true;
            frameQueue->push(frameData);
            auto *frameData2 = new AVFrameData();
            frameData->over = true;
            frameQueue->push(frameData2);

            break;
        }
        int re;
        if (packetData->packet && packetData->size > 0) {
            re = avcodec_send_packet(codecContext, packetData->packet);
            packetData->clear();
            if (re != 0) {
                ALOGE("avcodec_send_packet error")
                return false;
            }
//            ALOGD("avcodec_send_packet success")
            while (isDecoding) {
                AVFrame *frame = av_frame_alloc();
                re = avcodec_receive_frame(codecContext, frame);
                if (re == 0) {
//                    ALOGD("avcodec_receive_frame success");
                    auto *frameData = new AVFrameData();
                    frameData->frame = frame;

                    //resample
                    int size = 2 * frame->nb_samples * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
                    frameData->data = new unsigned char[size];
                    uint8_t *outArr[2] = {0};
                    outArr[0] = frameData->data;
                    int len = swr_convert(swrContext, outArr, frame->nb_samples,
                                          (const uint8_t **) frame->data, frame->nb_samples);
                    if (len <= 0) {
                        ALOGE("resample error");
                        frameData->clear();
                        continue;
                    }
                    frameData->size = size;
                    frameData->pts = frame->pts * 1000 * timeBase;
                    frameQueue->push(frameData);
                } else {
//                    ALOGD("avcodec_receive_frame fail");
                    av_frame_free(&frame);
                    break;
                }
            }
        }
    }
    ALOGD("FFDecode start break");
    return true;
}

bool FFDecode::pause() {
    return true;
}

void FFDecode::release() {
    if (codecContext) {
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
    }
    if (swrContext) {
        swr_free(&swrContext);
    }
    delete packetQueue;
    ALOGD("delete packetQueue success!");

    delete this;
}

FFDecode::~FFDecode() {
    ALOGD("delete FFDecode");
}
