//
// Created by templechen on 2019-09-06.
//

#ifndef HYPLAYER_FFMPEGENCODER_H
#define HYPLAYER_FFMPEGENCODER_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;

class FFmpegEncoder {

public:

    FFmpegEncoder();

    ~FFmpegEncoder();

    void encode();

private:

    void
    addStream(OutputStream *ost, AVFormatContext *oc, AVCodec **codec, enum AVCodecID codec_id);

    void openVideo(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg);

    AVFrame* allocPicture(enum AVPixelFormat pix_fmt, int width, int height);

    AVOutputFormat *fmt;
    AVFormatContext *oc;
    AVCodec *codec;
    AVCodecContext *codecContext;
    OutputStream *videoOutputStream;
    bool have_video = false;

    const int width = 720;
    const int height = 1280;
    const int bit_rate = 2000000;
    const int max_nFrames = 100;
    const int frame_rate = 30;
    const int gop = 100;
    const AVPixelFormat pix_fmt = AV_PIX_FMT_YUV420P;

};


#endif //HYPLAYER_FFMPEGENCODER_H
