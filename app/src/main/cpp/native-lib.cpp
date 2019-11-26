#include <jni.h>
#include <string>
#include <android/native_window.h>
#include <flutter/FlutterLooper.h>
#include <live/LiveEncoder.h>
#include <camera/NDKCamera.h>
#include <camera/CameraLooper.h>
#include <player/VideoPlayer.h>
#include <audio/SLRecoderLooper.h>
#include <obj/ObjViewerLooper.h>
#include "android/native_window_jni.h"
#include "player/AudioPlayer.h"
#include "template/TemplateLooper.h"
#include "base/native_log.h"
#include "encode/MediaCodecEncoder.h"
#include "encode/NDKCodecEncoder.h"
#include "encode/FFmpegCodecEncoder.h"

extern "C" {
#include "libavcodec/jni.h"
}

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
    return 0L;
}

extern "C" JNIEXPORT jlong JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeCurrentTime(
        JNIEnv *env,
        jobject instance) {
    if (audioPlayer != nullptr) {
        return audioPlayer->getCurrentDuration();
    }
    return 0L;
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeSetLoop(
        JNIEnv *env,
        jobject instance,
        jboolean loop) {
    if (audioPlayer != nullptr) {
        audioPlayer->setLoop(loop);
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeNext(
        JNIEnv *env,
        jobject instance,
        jstring url) {
    if (audioPlayer != nullptr) {
        const char *mediaUrl = env->GetStringUTFChars(url, nullptr);
        audioPlayer->setDataSource(mediaUrl);
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_audio_HYAudioPlayer_nativeSetVolume(
        JNIEnv *env,
        jobject instance,
        jint volume) {
    if (audioPlayer != nullptr) {
        audioPlayer->setVolume(volume);
    }
}

//.......................................................
//VideoPlayer
VideoPlayer *videoPlayer;
extern "C" JNIEXPORT bool JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeInit(
        JNIEnv *env,
        jobject instance,
        jstring url,
        jboolean usingMediaCodec) {
    const char *mediaUrl = env->GetStringUTFChars(url, nullptr);
    videoPlayer = new VideoPlayer(mediaUrl, usingMediaCodec);
    return true;
}

extern "C" JNIEXPORT bool JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeStart(
        JNIEnv *env,
        jobject instance) {
    if (videoPlayer != nullptr) {
        videoPlayer->start();
    }
    return true;
}

extern "C" JNIEXPORT bool JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativePause(
        JNIEnv *env,
        jobject instance) {
    if (videoPlayer != nullptr) {
        videoPlayer->pause();
    }
    return true;
}

extern "C" JNIEXPORT bool JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeSeek(
        JNIEnv *env,
        jobject instance,
        jlong pos) {
    if (videoPlayer != nullptr) {
        videoPlayer->seek(pos);
    }
    return true;
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeRelease(
        JNIEnv *env,
        jobject instance) {
    if (videoPlayer != nullptr) {
        videoPlayer->release();
    }
}

extern "C" JNIEXPORT jlong JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeTotalDuration(
        JNIEnv *env,
        jobject instance) {
    if (videoPlayer != nullptr) {
        return videoPlayer->getTotalDuration();
    }
    return 0L;
}

extern "C" JNIEXPORT jlong JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeCurrentTime(
        JNIEnv *env,
        jobject instance) {
    if (videoPlayer != nullptr) {
        return videoPlayer->getCurrentDuration();
    }
    return 0L;
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeSurfaceCreated(
        JNIEnv *env,
        jobject instance,
        jobject surface) {
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (videoPlayer != nullptr) {
        videoPlayer->setNativeWindowCreated(window);
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeSurfaceChanged(
        JNIEnv *env,
        jobject instance,
        jint width,
        jint height) {
    if (videoPlayer != nullptr) {
        videoPlayer->setNativeWindowChanged(width, height);
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeSurfaceDestroyed(
        JNIEnv *env,
        jobject instance) {
    if (videoPlayer != nullptr) {
        videoPlayer->setNativeWindowDestroyed();
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_cw_hyplayer_video_HYVideoPlayer_nativeDoFrame(
        JNIEnv *env,
        jobject instance) {
    if (videoPlayer != nullptr) {
        videoPlayer->doFrame();
    }
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
    av_jni_set_java_vm(vm, nullptr);
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


//......................................................
LiveEncoder *liveEncoder;
extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_CameraView_nativeCameraCreated(
        JNIEnv *env,
        jobject instance,
        jobject surface,
        jint windowWidth,
        jint windowHeight
) {
    liveEncoder = new LiveEncoder(javaVM);
    liveEncoder->sendMessage(liveEncoder->kLiveEncoderStart);
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_CameraView_nativeCameraChanged(
        JNIEnv *env,
        jobject instance,
        int width,
        int height
) {

}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_CameraView_nativeCameraDestroyed(
        JNIEnv *env,
        jobject instance
) {
    if (liveEncoder != nullptr) {
        liveEncoder->sendMessage(liveEncoder->kLiveEncoderStop);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_CameraView_nativeEncodeCameraData(
        JNIEnv *env,
        jobject instance,
        jbyteArray data,
        jint width,
        jint height,
        jlong pts
) {
    if (liveEncoder != nullptr) {
        jbyte *yuv420Buffer = env->GetByteArrayElements(data, 0);
        auto *buffer = static_cast<unsigned char *>(malloc((size_t) width * height * 5 / 4));
        memcpy(buffer, yuv420Buffer, width * height * 5 / 4);
        auto *nv21Data = new CameraNV21Data();
        nv21Data->data = reinterpret_cast<unsigned char *>(yuv420Buffer);
        nv21Data->width = width;
        nv21Data->height = height;
        nv21Data->pts = pts;
        liveEncoder->sendMessage(liveEncoder->kLiveEncoderDoFrame, nv21Data);
        env->ReleaseByteArrayElements(data, yuv420Buffer, 0);
    }
}

//......................................................
CameraLooper *cameraLooper;
jobject javaCameraView;
extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_NativeCameraView_nativeCameraCreated(
        JNIEnv *env,
        jobject instance,
        jobject surface,
        jobject cameraView
) {
    javaCameraView = env->NewGlobalRef(cameraView);
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    cameraLooper = new CameraLooper(window, javaVM, javaCameraView);
    cameraLooper->sendMessage(cameraLooper->kMsgCameraViewCreated);
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_NativeCameraView_nativeCameraChanged(
        JNIEnv *env,
        jobject instance,
        int width,
        int height
) {
    if (cameraLooper != nullptr) {
        cameraLooper->sendMessage(cameraLooper->kMsgCameraViewChanged, width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_NativeCameraView_nativeCameraSetSize(
        JNIEnv *env,
        jobject instance,
        int width,
        int height
) {
    if (cameraLooper != nullptr) {
        cameraLooper->sendMessage(cameraLooper->kMsgCameraSizeChanged, width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_NativeCameraView_nativeCameraDestroyed(
        JNIEnv *env,
        jobject instance
) {
    if (cameraLooper != nullptr) {
        cameraLooper->sendMessage(cameraLooper->kMsgCameraViewDestroyed);
        cameraLooper->quit();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_NativeCameraView_nativeCameraDoFrame(
        JNIEnv *env,
        jobject instance
) {
    if (cameraLooper != nullptr) {
        cameraLooper->sendMessage(cameraLooper->kMsgCameraViewDoFrame);
    }
}

CameraFFVideoEncoder *cameraFfVideoEncoder;
FFVideoEncoder *videoEncoder;
extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_NativeCameraView_nativeCameraFFEncodeStart(
        JNIEnv *env,
        jobject instance,
        int width,
        int height
) {
//    cameraFfVideoEncoder = new CameraFFVideoEncoder();
//    cameraFfVideoEncoder->sendMessage(cameraFfVideoEncoder->kMsgCameraEncodeStart);

    videoEncoder = new FFVideoEncoder();
    videoEncoder->InitEncoder("sdcard/trailer_test.mp4", 720, 1280);
    videoEncoder->EncodeStart();
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_NativeCameraView_nativeCameraFFEncodeFrame(
        JNIEnv *env,
        jobject instance,
        jbyteArray data,
        int width,
        int height
) {
//    if (cameraFfVideoEncoder != nullptr) {
//        ALOGD("nativeCameraFFEncodeFrame");
//        jbyte *yuv420Buffer = env->GetByteArrayElements(data, 0);
//        unsigned char *buffer = static_cast<unsigned char *>(malloc((size_t) width * height * 3 / 2));
//        memcpy(yuv420Buffer, yuv420Buffer, (size_t) width * height * 3 / 2);
//        cameraFfVideoEncoder->sendMessage(cameraFfVideoEncoder->kMsgCameraEncoderFrame, width, height, buffer);
//        env->ReleaseByteArrayElements(data, yuv420Buffer, 0);
//    }

    jbyte *yuv420Buffer = env->GetByteArrayElements(data, 0);
    videoEncoder->EncodeYUV420Buffer(reinterpret_cast<unsigned char *>(yuv420Buffer), width,
                                     height);
    env->ReleaseByteArrayElements(data, yuv420Buffer, 0);
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_camera_NativeCameraView_nativeCameraFFEncodeEnd(
        JNIEnv *env,
        jobject instance
) {
//    if (cameraFfVideoEncoder != nullptr) {
//        cameraFfVideoEncoder->sendMessage(cameraFfVideoEncoder->kMsgCameraEncoderEnd);
//    }
    videoEncoder->EncoderEnd();
}

//......................................................
SLRecoderLooper *recoderLooper = nullptr;
extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_encode_audio_AudioEncoder_nativeInitAudioEncoder(
        JNIEnv *env,
        jobject instance,
        jstring path
) {
    recoderLooper = new SLRecoderLooper();
    const char *audioPath = env->GetStringUTFChars(path, nullptr);
    recoderLooper->sendMessage(recoderLooper->kMsgSLRecoderCreated, (void *) audioPath);
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_encode_audio_AudioEncoder_nativeStartAudioEncoder(
        JNIEnv *env,
        jobject instance
) {
    if (recoderLooper != nullptr) {
        recoderLooper->sendMessage(recoderLooper->kMsgSLRecoderStart);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_encode_audio_AudioEncoder_nativePauseAudioEncoder(
        JNIEnv *env,
        jobject instance
) {
    if (recoderLooper != nullptr) {
        recoderLooper->sendMessage(recoderLooper->kMsgSLRecoderPause);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_encode_audio_AudioEncoder_nativeReleaseAudioEncoder(
        JNIEnv *env,
        jobject instance
) {
    if (recoderLooper != nullptr) {
        recoderLooper->sendMessage(recoderLooper->kMsgSLRecoderRelease);
        recoderLooper = nullptr;
    }
}

//......................................................
ObjViewerLooper *objViewerLooper = nullptr;
extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_viewer_NativeObjView_nativeObjViewCreated(
        JNIEnv *env,
        jobject instance,
        jobject surface
) {
    objViewerLooper = new ObjViewerLooper(ANativeWindow_fromSurface(env, surface));
    objViewerLooper->sendMessage(objViewerLooper->kMsgObjViewerCreated);
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_viewer_NativeObjView_nativeObjViewChanged(
        JNIEnv *env,
        jobject instance,
        jint width, jint height
) {
    if (objViewerLooper != nullptr) {
        objViewerLooper->sendMessage(objViewerLooper->kMsgObjViewerChanged, width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_viewer_NativeObjView_nativeObjViewDestroyed(
        JNIEnv *env,
        jobject instance
) {
    if (objViewerLooper != nullptr) {
        objViewerLooper->sendMessage(objViewerLooper->kMsgObjViewerDestroyed);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_cw_hyplayer_viewer_NativeObjView_nativeObjViewDoFrame(
        JNIEnv *env,
        jobject instance
) {
    if (objViewerLooper != nullptr) {
        objViewerLooper->sendMessage(objViewerLooper->kMsgObJViewerDoFrame);
    }
}
