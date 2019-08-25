package com.cw.hyplayer.audio

class HYAudioPlayer {

    val mediaSource: MediaSource

    constructor(mediaSource: MediaSource) {
        this.mediaSource = mediaSource
        nativeInit(mediaSource.url)
    }

    fun start(): Boolean {
        return nativeStart()
    }

    fun pause(): Boolean {
        return nativePause()
    }

    fun seek(pos: Long): Boolean {
        return nativeSeek(pos)
    }

    fun release() {
        nativeRelease()
    }

    private external fun nativeInit(url: String): Boolean

    private external fun nativeStart(): Boolean

    private external fun nativePause(): Boolean

    private external fun nativeSeek(pos: Long): Boolean

    private external fun nativeRelease()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}