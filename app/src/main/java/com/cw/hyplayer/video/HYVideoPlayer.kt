package com.cw.hyplayer.video

import android.view.Surface
import com.cw.hyplayer.audio.MediaSource

class HYVideoPlayer {

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

    fun totalDuration(): Long {
        return nativeTotalDuration()
    }

    fun currentTime(): Long {
        return nativeCurrentTime()
    }

    fun setSurface(surface: Surface) {
        nativeSetSurface(surface)
    }

    private external fun nativeInit(url: String): Boolean

    private external fun nativeStart(): Boolean

    private external fun nativePause(): Boolean

    private external fun nativeSeek(pos: Long): Boolean

    private external fun nativeRelease()

    private external fun nativeTotalDuration(): Long

    private external fun nativeCurrentTime(): Long

    private external fun nativeSetSurface(surface: Surface)

}