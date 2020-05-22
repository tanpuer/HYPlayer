package com.cw.hyplayer.video

import android.Manifest
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.Surface
import android.widget.SeekBar
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.cw.hyplayer.R
import com.cw.hyplayer.audio.MediaSource
import kotlinx.android.synthetic.main.activity_video.*
import java.util.*

class HYVideoActivity : AppCompatActivity(), IVideoViewCallback {

    private var videoPlayer: HYVideoPlayer? = null
    private var surface: Surface? = null
    private var surfaceWidth = 0
    private var surfaceHeight = 0
    private var settable = true

    private lateinit var timer: Timer
    private lateinit var handler: Handler

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_video)

        video_view.videoViewCallback = this

        create_soft_decoder.setOnClickListener {
            initPlayer(false)
        }
        create_hw_decoder.setOnClickListener {
            initPlayer(true)
        }
        start.setOnClickListener {
            videoPlayer?.start()
        }
        pause.setOnClickListener {
            videoPlayer?.pause()
        }
        release.setOnClickListener {
            videoPlayer?.release()
        }
        seek_bar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {

            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
                settable = false
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                if (seekBar == null || videoPlayer == null) {
                    return
                }
                val pos = videoPlayer!!.totalDuration() * seekBar.progress / seekBar.max
                Log.d("seek ", "$pos")
                videoPlayer?.seek(pos)
                settable = true
            }

        })

        handler = Handler(Looper.getMainLooper())
        timer = Timer()
        timer.scheduleAtFixedRate(object : TimerTask() {
            override fun run() {
                handler.post {
                    if (videoPlayer != null && settable && videoPlayer!!.totalDuration() > 0 && videoPlayer!!.currentTime() > 0) {
                        seek_bar.progress =
                            (videoPlayer!!.currentTime() * 100 / videoPlayer!!.totalDuration()).toInt()
                    }
                }
            }
        }, 0, 500)
        ActivityCompat.requestPermissions(
            this, arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE), 10001
        )
    }

    override fun surfaceCreated(surface: Surface) {
        this.surface = surface
    }

    override fun surfaceChanged(width: Int, height: Int) {
        surfaceWidth = width
        surfaceHeight = height
        videoPlayer?.setSurfaceChanged(surfaceWidth, surfaceHeight)
    }

    override fun surfaceDestroyed() {
        surface = null
        videoPlayer?.setSurfaceDestroyed()
        timer.cancel()
    }

    override fun surfaceDoFrame() {
        videoPlayer?.doFrame()
    }

    private fun initPlayer(usingMediaCodec: Boolean) {
        val mediaSource = MediaSource("/sdcard/trailer111.mp4")
        mediaSource.usingMediaCodec = usingMediaCodec
        videoPlayer = HYVideoPlayer(mediaSource)
        surface?.let {
            videoPlayer?.setSurfaceCreated(surface!!)
            if (surfaceWidth > 0 && surfaceHeight > 0) {
                videoPlayer?.setSurfaceChanged(surfaceWidth, surfaceHeight)
            }
        }
    }
}