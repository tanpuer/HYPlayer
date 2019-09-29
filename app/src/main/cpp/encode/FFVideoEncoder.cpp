//
// Created by templechen on 2019-09-10.
//

#include "FFVideoEncoder.h"
#include "../base/native_log.h"

extern "C" {
#include "libyuv.h"
}

void FFVideoEncoder::InitEncoder(const char *path, int width, int height) {
    this->mp4Path = path;
    this->width = width;
    this->height = height;
    ALOGD("FFVideoEncoder init finish");
    fp_out = fopen(this->mp4Path, "wb");
}

void FFVideoEncoder::EncodeStart() {
    av_register_all();
    //2. 初始化输出码流的AVFormatContext
    avformat_alloc_output_context2(&pFormatCtx, nullptr, nullptr, this->mp4Path);
    //3. 打开待输出的视频文件
    if (avio_open(&pFormatCtx->pb, this->mp4Path, AVIO_FLAG_READ_WRITE)) {
        ALOGE("open output file failed");
        return;
    }
    //4. 初始化视频码流
    pStream = avformat_new_stream(pFormatCtx, nullptr);
    if (pStream == nullptr) {
        ALOGE("allocating output stream fail");
        return;
    }
    //5. 寻找编码器并打开编码器
    pCodec = avcodec_find_encoder(AV_CODEC_ID_MPEG4);
    if (!pCodec) {
        ALOGE("could not find encoder");
        return;
    }
    //6. 分配编码器并设置参数
    pCodecCtx = avcodec_alloc_context3(pCodec);
    pCodecCtx->codec_id = pCodec->id;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = width;
    pCodecCtx->height = height;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = frame_rate;
    pCodecCtx->bit_rate = bit_rate;
    pCodecCtx->gop_size = gop_size;
    pCodecCtx->thread_count = 2;
    //将AVCodecContext的成员复制到AVCodecParameters结构体
    avcodec_parameters_from_context(pStream->codecpar, pCodecCtx);
    //打开编码器
    if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0) {
        ALOGE("open encoder fail");
        return;
    }
    //输出格式信息
    av_dump_format(pFormatCtx, 0, this->mp4Path, 1);
    //初始化帧
    pFrame = av_frame_alloc();
    pFrame->width = pCodecCtx->width;
    pFrame->height = pCodecCtx->height;
    pFrame->format = pCodecCtx->pix_fmt;
    int buffer_size = av_image_get_buffer_size(pCodecCtx->pix_fmt, pCodecCtx->width,
                                               pCodecCtx->height, 1);
    pFrameBuffer = static_cast<uint8_t *>(av_malloc(buffer_size));
    av_image_fill_arrays(pFrame->data, pFrame->linesize, pFrameBuffer, pCodecCtx->pix_fmt,
                         pCodecCtx->width, pCodecCtx->height, 1);
    //H264
    AVDictionary *opt = 0;
    if (pCodecCtx->codec_id == AV_CODEC_ID_H264) {
        av_dict_set_int(&opt, "video_track_timescale", 25, 0);
        av_dict_set(&opt, "preset", "slow", 0);
        av_dict_set(&opt, "true", "zerolatency", 0);
    }
    //写头文件
    avformat_write_header(pFormatCtx, &opt);
    //创建已编码帧
    av_new_packet(&avPacket, buffer_size * 3);

//    swsContext = sws_getContext(pCodecCtx->width, pCodecCtx->height,
//                                AV_PIX_FMT_RGBA,
//                                pCodecCtx->width, pCodecCtx->height,
//                                AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
    ALOGD("FFVideoEncoder start finish");

}

int i = 0;

void FFVideoEncoder::EncoderBuffer(unsigned char *buffer, long long pts) {
    //sws_scale is slower than libyuv

//    int rgbaLineSize[AV_NUM_DATA_POINTERS] = {0};
//    rgbaLineSize[0] = width * 4;
//    uint8_t *rgbaData[AV_NUM_DATA_POINTERS] = {0};
//    rgbaData[0] = buffer;
//    int res = sws_scale(swsContext, rgbaData, rgbaLineSize,
//                           0, pCodecCtx->height, pFrame->data, pFrame->linesize);
//    if (res <=0) {
//        ALOGE("sw_scale error!");
//        return;
//    }
//    ALOGD("sw_scale success %d", height);
//    pFrame->pts = i;
//    i++;
//    AVPacket avPacket;
//    av_init_packet(&avPacket);
//    EncoderFrame(pCodecCtx, pFrame, &avPacket);
//
//    free(buffer);

    //ABGR not RGBA!
    libyuv::ABGRToI420(buffer, width * 4, pFrame->data[0], width, pFrame->data[1], width / 2,
                       pFrame->data[2], width / 2,
                       width, height);
    pFrame->pts = i;
    i++;
    //编码数据
    EncoderFrame(pCodecCtx, pFrame, &avPacket);
    free(buffer);

}

void
FFVideoEncoder::EncoderNV21Buffer(unsigned char *buffer, int width, int height, long long pts) {
    ALOGD("EncoderNV21Buffer")
    libyuv::NV21ToI420(buffer, width, buffer + width * height, width, pFrame->data[0], this->width,
                       pFrame->data[1], this->width / 2, pFrame->data[2], this->width / 2, width,
                       height);


//    ALOGD("EncoderNV21Buffer %d %d %d %d", width, height, this->width, this->height);
//    libyuv::ConvertToI420(buffer, width * height, pFrame->data[0], this->width, pFrame->data[1],
//                          this->width / 2, pFrame->data[2], this->width / 2, 0, 0, height, width,
//                          this->width, this->height, libyuv::kRotate270, libyuv::FOURCC_NV21);

    pFrame->pts = i * 2;
    i++;
    EncoderFrame(pCodecCtx, pFrame, &avPacket);
}

void FFVideoEncoder::EncoderEnd() {
    int result = EncoderFrame(pCodecCtx, nullptr, &avPacket);
    if (result >= 0) {
        //封装文件尾
        ALOGD("av_write_trailer");
        av_write_trailer(pFormatCtx);
        //free
        if (pCodecCtx != nullptr) {
            avcodec_close(pCodecCtx);
            avcodec_free_context(&pCodecCtx);
            pCodecCtx = nullptr;
        }
        if (pFrame != nullptr) {
            av_free(pFrame);
            pFrame = nullptr;
        }
        if (pFrameBuffer != nullptr) {
            av_free(pFrameBuffer);
            pFrameBuffer = nullptr;
        }
        if (pFormatCtx != nullptr) {
            avio_close(pFormatCtx->pb);
            avformat_free_context(pFormatCtx);
            pFormatCtx = nullptr;
        }
    }
    ALOGD("FFVideoEncoder end");
}

int FFVideoEncoder::EncoderFrame(AVCodecContext *pCodecCtx, AVFrame *pFrame, AVPacket *avPacket) {
    ALOGD("EncoderFrame start");
    int ret = avcodec_send_frame(pCodecCtx, pFrame);
    if (ret < 0) {
        ALOGD("EncoderFrame fail");
        return -1;
    }
    while (!ret) {
        ret = avcodec_receive_packet(pCodecCtx, avPacket);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
//            ALOGD("EncoderFrame success");
            return 0;
        } else if (ret < 0) {
            //error during encoding
            ALOGD("EncoderFrame fail");
            return -1;
        }
        ALOGD("EncoderFrame success %d", avPacket->size);
//        fwrite(avPacket->data, 1, (size_t) avPacket->size, fp_out);
        avPacket->stream_index = pStream->index;
        av_packet_rescale_ts(avPacket, pCodecCtx->time_base, pStream->time_base);
        avPacket->pos = -1;
        int result = av_interleaved_write_frame(pFormatCtx, avPacket);
        if (result != 0) {
            ALOGD("EncoderFrame write packet fail");
        }
        ALOGD("EncoderFrame write packet success");
        av_packet_unref(avPacket);
    }
    return 0;
}
