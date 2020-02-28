//
// Created by templechen on 2019-11-25.
//

#include <base/native_log.h>
#include <assert.h>
#include "FFAudioEncoder.h"

FFAudioEncoder::FFAudioEncoder(const char *path) {
    this->path = path;
    avformat_alloc_output_context2(&pFormatCtx, nullptr, nullptr, this->path);
    if (avio_open(&pFormatCtx->pb, this->path, AVIO_FLAG_READ_WRITE) < 0) {
        ALOGE("open audio file failed %s", this->path);
        return;
    }
    outputFormat = pFormatCtx->oformat;
    pStream = avformat_new_stream(pFormatCtx, nullptr);
    if (pStream == nullptr) {
        ALOGE("create audio stream failed!");
        return;
    }
    pCodec = avcodec_find_encoder(outputFormat->audio_codec);
    if (!pCodec) {
        ALOGE("can not find aac encoder!");
        return;
    }
    pCodecCtx = pStream->codec;
    pCodecCtx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
    pCodecCtx->codec_id = pCodec->id;
    pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    pCodecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    pCodecCtx->sample_rate = 44100;
    pCodecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
    pCodecCtx->channels = av_get_channel_layout_nb_channels(pCodecCtx->channel_layout);
    pCodecCtx->bit_rate = 96000;
    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        pFormatCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    int result = avcodec_open2(pCodecCtx, pCodec, nullptr);
    if (result < 0) {
        ALOGE("can not open audio encoder!");
        return;
    }

    audioFrame = av_frame_alloc();
    audioFrame->nb_samples = pCodecCtx->frame_size;
    audioFrame->format = pCodecCtx->sample_fmt;
    bufferSize = av_samples_get_buffer_size(NULL, pCodecCtx->channels, pCodecCtx->frame_size,
                                            pCodecCtx->sample_fmt, 1);
    audioBuffer = static_cast<uint8_t *>(av_malloc(bufferSize));
    avcodec_fill_audio_frame(audioFrame, pCodecCtx->channels, pCodecCtx->sample_fmt, audioBuffer,
                             bufferSize, 1);

    avformat_write_header(pFormatCtx, nullptr);
    av_new_packet(&audioPacket, bufferSize);

    swr = swr_alloc();
    swr_alloc_set_opts(swr, pCodecCtx->channel_layout, pCodecCtx->sample_fmt,
                       pCodecCtx->sample_rate, AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_S16, 44100, 0, 0);
    int re = swr_init(swr);
    if (re != 0) {
        ALOGE("swr init failed!");
        return;
    }
    ALOGD("FFAudioEncoder create success!");
}

void FFAudioEncoder::encoderBuffer(short *buffer, int size) {
    uint8_t *outs[2];
    outs[0] = new uint8_t[size];
    outs[1] = new uint8_t[size];
    int count = swr_convert(swr, (uint8_t **) &outs, size * 4,
                            (const uint8_t **) &buffer, size / 4);
    audioFrame->data[0] = outs[0];
    audioFrame->data[1] = outs[1];

    if (count >= 0) {
        EncodeFrame(pCodecCtx, audioFrame);
    } else {
        char errorMessage[1024] = {0};
        av_strerror(size, errorMessage, sizeof(errorMessage));
        ALOGE("error message: %s", errorMessage);
    }
    delete outs[0];
    delete outs[1];
}

void FFAudioEncoder::encodeEnd() {
    EncodeFrame(pCodecCtx, NULL);
    //10.写文件尾
    av_write_trailer(pFormatCtx);
    release();
}

void FFAudioEncoder::release() {
    avcodec_close(pCodecCtx);
    av_free(audioFrame);
    av_free(audioBuffer);
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);
    swr_free(&swr);
}

void FFAudioEncoder::EncodeFrame(AVCodecContext *pCodecCtx, AVFrame *audioFrame) {
    assert(pCodecCtx != nullptr);
    assert(audioFrame != nullptr);
    int ret = avcodec_send_frame(pCodecCtx, audioFrame);
    if (ret < 0) {
        ALOGE("Could't send frame %d %d", audioFrame->channels, audioFrame->format);
        return;
    }
    while (avcodec_receive_packet(pCodecCtx, &audioPacket) == 0) {
        audioPacket.stream_index = pStream->index;
        ret = av_interleaved_write_frame(pFormatCtx, &audioPacket);
        av_packet_unref(&audioPacket);
        ALOGD("write packet success");
    }
}
