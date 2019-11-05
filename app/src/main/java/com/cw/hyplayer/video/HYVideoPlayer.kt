package com.cw.hyplayer.video

import android.view.Surface
import com.cw.hyplayer.audio.MediaSource

class HYVideoPlayer {

    private val mediaSource: MediaSource
    private var enable = false

    constructor(mediaSource: MediaSource) {
        this.mediaSource = mediaSource
        nativeInit(mediaSource.url, mediaSource.usingMediaCodec)
        enable = true
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
        enable = false
        nativeRelease()
    }

    fun totalDuration(): Long {
        return nativeTotalDuration()
    }

    fun currentTime(): Long {
        return nativeCurrentTime()
    }

    fun setSurfaceCreated(surface: Surface) {
        nativeSurfaceCreated(surface)
    }

    fun setSurfaceChanged(width: Int, height: Int) {
        nativeSurfaceChanged(width, height)
    }

    fun setSurfaceDestroyed() {
        nativeSurfaceDestroyed()
    }

    fun doFrame() {
        if (enable) {
            nativeDoFrame()
        }
    }

    private external fun nativeInit(url: String, usingMediaCodec: Boolean): Boolean

    private external fun nativeStart(): Boolean

    private external fun nativePause(): Boolean

    private external fun nativeSeek(pos: Long): Boolean

    private external fun nativeRelease()

    private external fun nativeTotalDuration(): Long

    private external fun nativeCurrentTime(): Long

    private external fun nativeSurfaceCreated(surface: Surface)

    private external fun nativeSurfaceChanged(width: Int, height: Int)

    private external fun nativeSurfaceDestroyed()

    private external fun nativeDoFrame()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

}