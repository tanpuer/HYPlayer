//
// Created by templechen on 2019-08-20.
//

#include "SLAudioPlayer.h"
#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"
#include "../base/native_log.h"

static SLObjectItf engineSL = NULL;
static SLEngineItf eng = NULL;
static SLObjectItf mix = NULL;
static SLObjectItf player = NULL;
static SLPlayItf iplayer = NULL;
static SLAndroidSimpleBufferQueueItf pcmQue = NULL;
static SLVolumeItf volumeItf = NULL;

static SLEngineItf CreateSL() {
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if (re != SL_RESULT_SUCCESS) return NULL;
    return en;
}

static void PcmCall(SLAndroidSimpleBufferQueueItf bf, void *contex) {
    SLAudioPlayer *ap = (SLAudioPlayer *) contex;
    ap->call((void *) bf);
}

SLAudioPlayer::SLAudioPlayer(circle_av_frame_queue *frameQueue) {
    this->frameQueue = frameQueue;
    buf = new unsigned char[1024 * 1024];

    //1 创建引擎
    eng = CreateSL();
    if (eng) {
        ALOGD("CreateSL success！ ");
    } else {
        ALOGE("CreateSL failed！ ");
    }
    //2 创建混音器

    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("SL_RESULT_SUCCESS failed!");
        return;
    }
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("(*mix)->Realize failed!");
        return;
    }
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outmix, 0};

    //3 配置音频信息
    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            (SLuint32) 2,//    声道数
            (SLuint32) 44100 * 1000,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN //字节序，小端
    };
    SLDataSource ds = {&que, &pcm};
    //4 创建播放器
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME, SL_IID_PLAY};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audioSink,
                                   sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("CreateAudioPlayer failed!");
        return;
    } else {
        ALOGD("CreateAudioPlayer success!");
    }
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    //获取player接口
    re = (*player)->GetInterface(player, SL_IID_PLAY, &iplayer);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("GetInterface SL_IID_PLAY failed!");
        return;
    }
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQue);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("GetInterface SL_IID_BUFFERQUEUE failed!");
        return;
    }
    re = (*player)->GetInterface(player, SL_IID_VOLUME, &volumeItf);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("GetInterface SL_IID_VOLUME failed!");
        return;
    }

    //设置回调函数，播放队列空调用
    (*pcmQue)->RegisterCallback(pcmQue, PcmCall, this);
}

SLAudioPlayer::~SLAudioPlayer() {
    ALOGD("delete SLAudioPlayer");
    delete[] buf;
}

void SLAudioPlayer::start() {
    ALOGD("SLAudioPlayer start!");
    //设置为播放状态
    (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_PLAYING);

    //启动队列回调
    (*pcmQue)->Enqueue(pcmQue, "", 1);
}

void SLAudioPlayer::pause() {
    ALOGD("SLAudioPlayer pause!");
    (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_PAUSED);
}

void SLAudioPlayer::call(void *bufq) {
    if (!bufq) {
        return;
    }
    AVFrameData *frameData = frameQueue->pull();
//    ALOGD("pull AVFrame from FrameQueue!");
    if (frameData == nullptr) {
        return;
    }
    if (frameData->over) {
        ALOGD("AVFrame is over!");
        frameData->clear();
        return;
    }
    currentPos = frameData->pts;
    memcpy(buf, frameData->data, frameData->size);
    if (pcmQue && (*pcmQue)) {
        (*pcmQue)->Enqueue(pcmQue, buf, frameData->size);
    }
    frameData->clear();
}

void SLAudioPlayer::init() {

}

void SLAudioPlayer::seek() {
    pause();
    AVFrameData *data = frameQueue->pull();
    while (!data->seekOver) {
        data->clear();
        data = frameQueue->pull();
    }
    data->clear();
    start();
}

void SLAudioPlayer::reset() {
    pause();
    AVFrameData *data = frameQueue->pull();
    while (!data->reset) {
        data->clear();
        data = frameQueue->pull();
    }
    data->clear();
    start();
}

void SLAudioPlayer::release() {
    ALOGD("SLAudioPlayer release");
    //停止播放
    if (iplayer && (*iplayer)) {
        (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_STOPPED);
    }
    //清空缓冲队列
    if (pcmQue && (*pcmQue)) {
        (*pcmQue)->Clear;
    }
    //销毁player对象
    if (player && (*player)) {
        (*player)->Destroy(player);
    }
    //销毁混音器
    if (mix && (*mix)) {
        (*mix)->Destroy(mix);
    }
    //销魂播放引擎
    if (engineSL && (*engineSL)) {
        (*engineSL)->Destroy(engineSL);
    }
    engineSL = NULL;
    eng = NULL;
    mix = NULL;
    player = NULL;
    iplayer = NULL;
    pcmQue = NULL;

    AVFrameData *data = frameQueue->pull();
    while (!data->over) {
        data->clear();
        data = frameQueue->pull();
    }
    data->clear();

    delete frameQueue;
    ALOGD("delete freamQueue success!")

    delete this;

}

void SLAudioPlayer::setVolume(float volume) {
    if (volumeItf != nullptr) {
        SLmillibel level = getAmplificationLevel(volume);
        SLresult result = (*volumeItf)->SetVolumeLevel(volumeItf, level);
        if (result != SL_RESULT_SUCCESS) {
            ALOGE("SLAudioPlayer setVolume failed!");
        }
    }
}

/**
 * 计算SL的音量
 * @param volumeLevel
 * @return
 */
SLmillibel SLAudioPlayer::getAmplificationLevel(float volumeLevel) {
    if (volumeLevel < 0.00000001) {
        return SL_MILLIBEL_MIN;
    }
    SLmillibel mb = lroundf(2000.f * log10f(volumeLevel));
    if (mb < SL_MILLIBEL_MIN) {
        mb = SL_MILLIBEL_MIN;
    } else if (mb > 0) {
        mb = 0;
    }
    return mb;
}
