package com.cw.hyplayer.encode.audio

class AudioEncoder(path: String) {

    init {
        nativeInitAudioEncoder(path)
    }

    fun start() {
        nativeStartAudioEncoder()
    }

    fun release() {
        nativeReleaseAudioEncoder()
    }

    private external fun nativeInitAudioEncoder(path: String)
    private external fun nativeStartAudioEncoder()
    private external fun nativeReleaseAudioEncoder()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}