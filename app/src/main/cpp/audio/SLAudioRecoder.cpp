//
// Created by templechen on 2019-11-25.
//

#include "SLAudioRecoder.h"
#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"
#include "../base/native_log.h"
#include "assert.h"

static SLObjectItf engineSL = NULL;
static SLEngineItf eng = NULL;
static SLObjectItf mix = NULL;
static SLObjectItf recoder = NULL;
static SLRecordItf iRecoder = NULL;
static SLAndroidSimpleBufferQueueItf pcmQue = NULL;

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
    assert(contex != nullptr);
    auto *ap = (SLAudioRecoder *) contex;
    ap->call((void *) bf);
}

SLAudioRecoder::SLAudioRecoder(const char *path) {
    this->path = path;

    //1 创建引擎
    eng = CreateSL();
    if (eng) {
        ALOGD("CreateSL success！ ");
    } else {
        ALOGE("CreateSL failed！ ");
    }
    SLresult re = 0;

    //3 配置音频信息
    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            1,//    声道数
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_CENTER,
            SL_BYTEORDER_LITTLEENDIAN //字节序，小端
    };
    SLDataLocator_IODevice locDevice = {SL_DATALOCATOR_IODEVICE, SL_IODEVICE_AUDIOINPUT,
                                        SL_DEFAULTDEVICEID_AUDIOINPUT,
                                        nullptr};
    SLDataSource audioSrc = {&locDevice, nullptr};
    SLDataSink audioSink = {&que, &pcm};

    //4 创建录制
    const SLInterfaceID ids[] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioRecorder(eng, &recoder, &audioSrc, &audioSink,
                                     sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("CreateAudioRecorder failed");
        return;
    }
    (*recoder)->Realize(recoder, SL_BOOLEAN_FALSE);
    //获取recoeder接口
    re = (*recoder)->GetInterface(recoder, SL_IID_RECORD, &iRecoder);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("GetInterface SL_IID_RECORD failed!");
        return;
    }
    re = (*recoder)->GetInterface(recoder, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &pcmQue);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("GetInterface SL_IID_ANDROIDSIMPLEBUFFERQUEUE failed!");
        return;
    }
    re = (*pcmQue)->RegisterCallback(pcmQue, PcmCall, this);
    if (re != SL_RESULT_SUCCESS) {
        ALOGE("RegisterCallback failed!");
        return;
    }
    mBuffers[0] = new short[BUFFER_SIZE];
    mBuffers[1] = new short[BUFFER_SIZE];
    ALOGD("CreateAudioRecorder success");

    audioEncoder = new FFAudioEncoder(path);
}

void SLAudioRecoder::start() {
    ALOGD("SLAudioRecoder start");
    (*iRecoder)->SetRecordState(iRecoder, SL_RECORDSTATE_RECORDING);
    (*pcmQue)->Clear(pcmQue);
    (*pcmQue)->Enqueue(pcmQue, mBuffers[mIndex], BUFFER_SIZE * sizeof(short));
}

void SLAudioRecoder::pause() {
    ALOGD("SLAudioRecoder pause");
    (*iRecoder)->SetRecordState(iRecoder, SL_RECORDSTATE_PAUSED);
}

void SLAudioRecoder::release() {
    ALOGD("SLAudioRecoder release");
    if (iRecoder && (*iRecoder)) {
        (*iRecoder)->SetRecordState(iRecoder, SL_RECORDSTATE_STOPPED);
    }
    if (pcmQue && (*pcmQue)) {
        (*pcmQue)->Clear;
    }
    if (recoder && (*recoder)) {
        (*recoder)->Destroy(recoder);
    }
    if (mix && (*mix)) {
        (*mix)->Destroy(mix);
    }
    if (engineSL && (*engineSL)) {
        (*engineSL)->Destroy(engineSL);
    }
    engineSL = nullptr;
    eng = nullptr;
    mix = nullptr;
    recoder = nullptr;
    iRecoder = nullptr;
    pcmQue = nullptr;

    if (mBuffers[0]) {
        delete[] mBuffers[0];
        mBuffers[0] = nullptr;
    }

    if (mBuffers[1]) {
        delete[] mBuffers[1];
        mBuffers[1] = nullptr;
    }

    audioEncoder->encodeEnd();
}

void SLAudioRecoder::call(void *bufq) {
    ALOGD("recoder frame call");
    audioEncoder->encoderBuffer(mBuffers[mIndex], BUFFER_SIZE);
    mIndex = 1 - mIndex;
    (*pcmQue)->Enqueue(pcmQue, mBuffers[mIndex], BUFFER_SIZE * sizeof(short));
}
