//
// Created by templechen on 2019-09-06.
//

#include "FFmpegEncoder.h"
#include "../base/native_log.h"

FFmpegEncoder::FFmpegEncoder() {
    av_register_all();
    AVDictionary *opt = nullptr;
    avformat_alloc_output_context2(&oc, nullptr, nullptr, "sdcard/trailer_test.mp4");
    if (!oc) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&oc, nullptr, "mpeg", "sdcard/trailer_test.mp4");
    }
    if (!oc) {
        ALOGE("alloc output context fail!");
        return;
    }
    fmt = oc->oformat;
    if (fmt->video_codec != AV_CODEC_ID_NONE) {
        addStream(videoOutputStream, oc, &codec, fmt->video_codec);
        have_video = true;
    }
    /* Now that all the parameters are set, we can open the audio and
     * video codecs and allocate the necessary encode buffers. */
    if (have_video) {
        openVideo(oc, codec, videoOutputStream, opt);
    }
    //todo
}

FFmpegEncoder::~FFmpegEncoder() {

}

void FFmpegEncoder::encode() {

}

void FFmpegEncoder::addStream(OutputStream *ost, AVFormatContext *oc, AVCodec **codec,
                              enum AVCodecID codec_id) {
    AVCodecContext *c;
    int i;
    *codec = avcodec_find_decoder(codec_id);
    if (!(*codec)) {
        ALOGE(stderr, "Could not find encoder for '%s'\n",
              avcodec_get_name(codec_id));
        return;
    }
    ost->st = avformat_new_stream(oc, *codec);
    if (!ost->st) {
        ALOGE("could not allocate stream!");
        return;
    }
    ost->st->id = oc->nb_streams - 1;
    c = ost->st->codec;
    switch ((*codec)->type) {
        case AVMEDIA_TYPE_VIDEO: {
            c->codec_id = codec_id;
            c->bit_rate = bit_rate;
            c->width = width;
            c->height = height;
            ost->st->time_base = AVRational{1, frame_rate};
            c->time_base = ost->st->time_base;
            c->gop_size = gop;
            c->pix_fmt = pix_fmt;

            break;
        }
        default: {
            ALOGD("unsupported media type!");
            return;
        }
    }
    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

void FFmpegEncoder::openVideo(AVFormatContext *oc, AVCodec *codec, OutputStream *ost,
                              AVDictionary *opt_arg) {
    int ret;
    AVCodecContext *c = ost->st->codec;
    AVDictionary *opt = nullptr;
    av_dict_copy(&opt, opt_arg, 0);
    /* open the codec */
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        ALOGE("could not open video codec");
        return;
    }
    /* allocate and init a re-usable frame */
    ost->frame = allocPicture(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        ALOGE("could not alloc video frame");
        return;
    }
    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    ost->tmp_frame = nullptr;
    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        ost->tmp_frame = allocPicture(AV_PIX_FMT_YUV420P, c->width, c->height);
        if (!ost->tmp_frame) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
    }
}

AVFrame *FFmpegEncoder::allocPicture(enum AVPixelFormat pix_fmt, int width, int height) {
    AVFrame *picture;
    int ret;
    picture = av_frame_alloc();
    if (!picture) {
        return nullptr;
    }
    picture->format = pix_fmt;
    picture->width = width;
    picture->height = height;
    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        ALOGE("could not alloc frame data.");
        return nullptr;
    }
    return picture;
}
