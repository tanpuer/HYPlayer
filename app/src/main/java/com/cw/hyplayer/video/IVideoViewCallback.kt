package com.cw.hyplayer.video

import android.view.Surface

interface IVideoViewCallback {

    fun surfaceCreated(surface: Surface) {

    }

    fun surfaceChanged(width: Int, height: Int) {

    }

    fun surfaceDestroyed() {

    }

    fun surfaceDoFrame() {

    }

}