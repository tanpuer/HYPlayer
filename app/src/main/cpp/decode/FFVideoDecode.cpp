//
// Created by templechen on 2019-10-18.
//

#include <base/native_log.h>
#include "assert.h"
#include "FFVideoDecode.h"

FFVideoDecode::FFVideoDecode(bool usingMediaCodec) {
    this->usingMediaCodec = usingMediaCodec;
}

FFVideoDecode::~FFVideoDecode() {
    
}

bool FFVideoDecode::init() {
    return true;
}

bool FFVideoDecode::start() {
    while (isDecoding) {
        if (parameters == nullptr) {
            ALOGD("FFVideoDecode init start");
            AVPacketData *packetData = packetQueue->pull();
            if (packetData == nullptr) {
                continue;
            }
            parameters = packetData->parameters;
            timeBase = packetData->timeBase;
            if (usingMediaCodec) {
                codec = avcodec_find_decoder_by_name("h264_mediacodec");
                if (codec == nullptr) {
                    ALOGE("h264_mediacodec init error, change to soft decoder");
                    codec = avcodec_find_decoder(parameters->codec_id);
                }
            } else {
                codec = avcodec_find_decoder(parameters->codec_id);
            }
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
            ALOGD("FFVideoDecode init finish");
        }
        ALOGD("pull AVPacket data from packetQueue!");

        AVPacketData *packetData = nullptr;
        if (this->packetData != nullptr) {
            packetData = this->packetData;
        } else {
            packetData = packetQueue->pull();
        }
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
            assert(codecContext != nullptr);
            re = avcodec_send_packet(codecContext, packetData->packet);
            if (re == 0) {
                packetData->clear();
                this->packetData = nullptr;
            } else if (re == AVERROR((EAGAIN))) {
                //???
                this->packetData = packetData;
            }else if (re < 0 && re != AVERROR_EOF) {
                char buf[1024] = {0};
                av_strerror(re, buf, sizeof(buf) - 1);
                ALOGE("avcodec_send_packet error !%s", buf);
                return false;
            }
            while (isDecoding) {
                AVFrame *frame = av_frame_alloc();
                re = avcodec_receive_frame(codecContext, frame);
                if (re == 0) {
                    auto *frameData = new AVFrameData();
                    frameData->frame = frame;
                    frameData->size =
                            (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) *
                            frame->height;;
                    frameData->pts = frame->pts * 1000 * timeBase;
                    frameQueue->push(frameData);
                } else {
                    av_frame_free(&frame);
                    break;
                }
            }
        }
    }
    ALOGD("FFDecode start break");
    return true;
}

bool FFVideoDecode::pause() {
    return true;
}

void FFVideoDecode::release() {
    if (codecContext) {
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
        ALOGD("FFVideoDecode release success!");
    }
    delete packetQueue;
    ALOGD("delete packetQueue success!");

    delete this;
}
