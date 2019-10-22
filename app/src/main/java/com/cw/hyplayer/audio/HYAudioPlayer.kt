package com.cw.hyplayer.audio

class HYAudioPlayer {

    private val mediaSource: MediaSource

    var loop = false
        set(value) {
            field = value
            nativeSetLoop(value)
        }

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

    fun totalDuration(): Long {
        return nativeTotalDuration()
    }

    fun currentTime(): Long {
        return nativeCurrentTime()
    }

    private external fun nativeInit(url: String): Boolean

    private external fun nativeStart(): Boolean

    private external fun nativePause(): Boolean

    private external fun nativeSeek(pos: Long): Boolean

    private external fun nativeRelease()

    private external fun nativeTotalDuration(): Long

    private external fun nativeCurrentTime(): Long

    private external fun nativeSetLoop(loop: Boolean)

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

}