#include <jni.h>
#include <string>
#include <android/native_window.h>
#include <flutter/FlutterLooper.h>
#include "android/native_window_jni.h"
#include "player/AudioPlayer.h"
#include "template/TemplateLooper.h"
#include "base/native_log.h"
#include "encode/MediaCodecEncoder.h"
#include "encode/NDKCodecEncoder.h"
#include "encode/FFmpegCodecEncoder.h"

JavaVM *javaVM;

//.......................................................
//AudioPlayer

AudioPlayer *audioPlayer;

extern "C" JNIEXPORT jboolean JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeInit(
        JNIEnv *env,
        jobject instance,
        jstring url) {
    const char *mediaUrl = env->GetStringUTFChars(url, nullptr);
    audioPlayer = new AudioPlayer(mediaUrl);
    return true;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeStart(
        JNIEnv *env,
        jobject instance) {
    if (audioPlayer != nullptr) {
        audioPlayer->start();
    }
    return true;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativePause(
        JNIEnv *env,
        jobject instance) {
    if (audioPlayer != nullptr) {
        audioPlayer->pause();
    }
    return true;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeSeek(
        JNIEnv *env,
        jobject instance,
        jlong pos) {
    if (audioPlayer != nullptr) {
        audioPlayer->seek(pos);
    }
    return true;
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeRelease(
        JNIEnv *env,
        jobject instance) {
    if (audioPlayer != nullptr) {
        audioPlayer->release();
    }
}

extern "C" JNIEXPORT jlong JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeTotalDuration(
        JNIEnv *env,
        jobject instance) {
    if (audioPlayer != nullptr) {
        return audioPlayer->getTotalDuration();
    }
}

extern "C" JNIEXPORT jlong JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeCurrentTime(
        JNIEnv *env,
        jobject instance) {
    if (audioPlayer != nullptr) {
        return audioPlayer->getCurrentDuration();
    }
}

//.......................................................
//VideoPlayer

extern "C" JNIEXPORT jboolean JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeInit(
        JNIEnv *env,
        jobject instance,
        jstring url) {

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeStart(
        JNIEnv *env,
        jobject instance) {

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativePause(
        JNIEnv *env,
        jobject instance) {

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeSeek(
        JNIEnv *env,
        jobject instance,
        jlong pos) {

    return true;
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeRelease(
        JNIEnv *env,
        jobject instance) {

}

extern "C" JNIEXPORT jlong JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeTotalDuration(
        JNIEnv *env,
        jobject instance) {

    return 0L;
}

extern "C" JNIEXPORT jlong JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeCurrentTime(
        JNIEnv *env,
        jobject instance) {

    return 0L;
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeSetSurface(
        JNIEnv *env,
        jobject instance,
        jobject surface) {

}

//.......................................................
//Template

ANativeWindow *nativeWindow;
TemplateLooper *templateLooper;
extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_template_TemplateView_nativeTemplateViewCreated(
        JNIEnv *env,
        jobject instance,
        jobject surface) {
    nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (templateLooper != nullptr) {
        delete templateLooper;
        templateLooper = nullptr;
    }
    ALOGD("TemplateLooper is null %d", templateLooper == nullptr);
    templateLooper = new TemplateLooper(nativeWindow);
    templateLooper->sendMessage(templateLooper->kMsgTemplateCreated);
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_template_TemplateView_nativeTemplateViewChanged(
        JNIEnv *env,
        jobject instance,
        jint width,
        jint height) {
    if (templateLooper != nullptr) {
        templateLooper->sendMessage(templateLooper->kMsgTemplateChanged, width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_template_TemplateView_nativeTemplateViewDestroyed(
        JNIEnv *env,
        jobject instance) {
    if (templateLooper != nullptr) {
        templateLooper->sendMessage(templateLooper->kMsgTemplateDestroyed);
        templateLooper->quit();
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_template_TemplateView_nativeTemplateDoFrame(
        JNIEnv *env,
        jobject instance,
        jlong frameTimeNanos) {
    if (templateLooper != nullptr) {
        templateLooper->sendMessage(templateLooper->kMsgTemplateDoFrame,
                                    reinterpret_cast<void *>(frameTimeNanos));
    }
}

//......................................................
//System
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    javaVM = vm;
    //ffmpeg mediacodec
    return JNI_VERSION_1_6;
}

//......................................................
//JavaMediaEncoder
MediaCodecEncoder *mediaCodecEncoder;
jobject globalSurface;
jobject globalJavaMediaEncoder;
extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_encode_JavaMediaEncoder_nativeInit(
        JNIEnv *env,
        jobject instance,
        jobject surface,
        jobject javaMediaEncoder
) {
    globalSurface = env->NewGlobalRef(surface);
    globalJavaMediaEncoder = env->NewGlobalRef(javaMediaEncoder);
    mediaCodecEncoder = new MediaCodecEncoder(globalSurface, globalJavaMediaEncoder, javaVM);
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_encode_JavaMediaEncoder_nativeCancel(
        JNIEnv *env,
        jobject instance
) {

}

//......................................................
//NDKMediaCodecEncoder
NDKCodecEncoder *ndkCodecEncoder;
extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_encode_NDKMediaEncoder_nativeInit(
        JNIEnv *env,
        jobject instance,
        jint width,
        jint height,
        jint bitRate,
        jstring path
) {
    const char *url = env->GetStringUTFChars(path, nullptr);
    ndkCodecEncoder = new NDKCodecEncoder(width, height, bitRate, url, javaVM);
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_encode_NDKMediaEncoder_nativeCancel(
        JNIEnv *env,
        jobject instance
) {

}

//......................................................
//FFmpegMediaEncoder
FFmpegCodecEncoder *ffmpegCodecEncoder;
extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_encode_FFmpegMediaEncoder_nativeInit(
        JNIEnv *env,
        jobject instance,
        jint width,
        jint height,
        jint bitRate,
        jstring path
) {
    const char *url = env->GetStringUTFChars(path, nullptr);
    ffmpegCodecEncoder = new FFmpegCodecEncoder(width, height, bitRate, url, javaVM);
}


//......................................................
//FlutterView

FlutterLooper *flutterLooper;
extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_flutter_FlutterView_nativeFlutterViewCreated(
        JNIEnv *env,
        jobject instance,
        jobject surface) {
    nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (flutterLooper != nullptr) {
        delete flutterLooper;
        flutterLooper = nullptr;
    }
    flutterLooper = new FlutterLooper(nativeWindow);
    flutterLooper->sendMessage(flutterLooper->kMsgFlutterCreated);
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_flutter_FlutterView_nativeFlutterViewChanged(
        JNIEnv *env,
        jobject instance,
        jint width,
        jint height) {
    if (flutterLooper != nullptr) {
        flutterLooper->sendMessage(flutterLooper->kMsgFlutterChanged, width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_flutter_FlutterView_nativeFlutterViewDestroyed(
        JNIEnv *env,
        jobject instance) {
    if (flutterLooper != nullptr) {
        flutterLooper->sendMessage(flutterLooper->kMsgFlutterDestroyed);
        flutterLooper->quit();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_flutter_FlutterView_nativeFlutterViewDoFrame(
        JNIEnv *env,
        jobject instance,
        jlong frameTimeNanos) {
    if (flutterLooper != nullptr) {
        flutterLooper->sendMessage(flutterLooper->kMsgFlutterDoFrame,
                                   reinterpret_cast<void *>(frameTimeNanos));
    }
}