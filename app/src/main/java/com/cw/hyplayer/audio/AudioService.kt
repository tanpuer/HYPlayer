package com.cw.hyplayer.audio

import android.app.Service
import android.content.Intent
import android.os.IBinder

class AudioService : Service() {

    var audioPlayer: HYAudioPlayer? = null

    override fun onBind(intent: Intent?): IBinder? {
        return null
    }

    fun create() {
        if (audioPlayer == null) {
            val mediaSource = MediaSource("/sdcard/test.mp3")
//                val mediaSource = MediaSource("/sdcard/trailer111.mp4")
            audioPlayer = HYAudioPlayer(mediaSource)
        }
    }

    fun release() {
        audioPlayer?.release()
        audioPlayer = null
    }

    fun start() {
        audioPlayer?.start()
    }

    fun pause() {
        audioPlayer?.start()
    }

    fun seek(pos: Long) {
        audioPlayer?.seek(pos)
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        when (intent?.extras?.getSerializable("key")) {
            CREATE -> {
                create()
            }
            START -> {
                start()
            }
            RELEASE -> {
                release()
            }
            PAUSE -> {
                pause()
            }
            SEEK -> {
                seek(intent.extras?.getSerializable("pos") as Long)
            }
        }
        return super.onStartCommand(intent, flags, startId)
    }

    companion object {
        const val CREATE = "create"
        const val START = "start"
        const val RELEASE = "release"
        const val PAUSE = "pause"
        const val SEEK = "seek"
    }

}