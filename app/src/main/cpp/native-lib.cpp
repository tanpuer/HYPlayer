#include <jni.h>
#include <string>
#include "player/AudioPlayer.h"

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



