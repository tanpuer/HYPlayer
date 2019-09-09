//
// Created by templechen on 2019-09-09.
//

#include "VideoEncoder.h"
#include "../base/native_log.h"
#include <OMXAL/OpenMAXAL.h>
#include <OMXAL/OpenMAXAL_Android.h>
#include <stdio.h>
#include <cstring>
#include <android/native_window.h>


VideoEncoder::VideoEncoder() {
    ALOGD("VideoEncoder init start");
    codec = AMediaCodec_createEncoderByType("video/avc");
    bufferInfo = new AMediaCodecBufferInfo();
    format = AMediaFormat_new();

    AMediaFormat_setString(format, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_HEIGHT, 1280);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_WIDTH, 720);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_BIT_RATE, 2000000);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 1);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_FRAME_RATE, 30);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, 0x7F000789);

    media_status_t rc = AMediaCodec_configure(codec, format, nullptr, nullptr,
                                              AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    if (AMEDIA_OK == rc) {
        //todo api 26
        AMediaCodec_createInputSurface(codec, &nativeWindow);
        AMediaCodec_start(codec);
    } else {
        ALOGE("VideoEncoder init error");
    }
    ALOGD("VideoEncoder init finished");
    file = fopen("sdcard/trailer_test.mp4", "w+");
    muxer = AMediaMuxer_new(fileno(file), AMEDIAMUXER_OUTPUT_FORMAT_MPEG_4);
}

VideoEncoder::~VideoEncoder() {
    delete bufferInfo;
    AMediaFormat_delete(format);
}

void VideoEncoder::drainEncoderWithNoTimeOut(bool endOfStream) {
    if (endOfStream) {
        AMediaCodec_signalEndOfInputStream(codec);
    }
    while (true) {
        int status = AMediaCodec_dequeueOutputBuffer(codec, bufferInfo, 0);
        if (status == AMEDIACODEC_INFO_TRY_AGAIN_LATER) {
            if (!endOfStream) {
                break;
            } else {
                ALOGD("no output available, spinning to await EOS");
            }
        } else if (status == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
            AMediaFormat *format = AMediaCodec_getOutputFormat(codec);
            trackIndex = AMediaMuxer_addTrack(muxer, format);
            AMediaMuxer_start(muxer);
        } else if (status < 0) {
            //ignore
        } else {
            size_t bufsize;
            uint8_t *encodedData = AMediaCodec_getOutputBuffer(codec, status, &bufsize);
            if (bufferInfo->size > 0) {
                AMediaMuxer_writeSampleData(muxer, trackIndex, encodedData, bufferInfo);
            }
            AMediaCodec_releaseOutputBuffer(codec, status, false);
            if (bufferInfo->flags == AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
                break;
            }
        }
    }
}

void VideoEncoder::release() {
    ANativeWindow_release(nativeWindow);
    AMediaCodec_stop(codec);
    AMediaCodec_delete(codec);
    AMediaMuxer_stop(muxer);
    AMediaMuxer_delete(muxer);
    fclose(file);
}
