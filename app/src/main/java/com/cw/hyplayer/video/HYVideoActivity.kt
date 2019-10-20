package com.cw.hyplayer.video

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.Surface
import android.widget.SeekBar
import androidx.appcompat.app.AppCompatActivity
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
            val mediaSource = MediaSource("/sdcard/trailer.mp4")
            videoPlayer = HYVideoPlayer(mediaSource)
            surface?.let {
                videoPlayer?.setSurfaceCreated(surface!!)
                if (surfaceWidth > 0 && surfaceHeight > 0) {
                    videoPlayer?.setSurfaceChanged(surfaceWidth, surfaceHeight)
                }
            }
        }
        create_hw_decoder.setOnClickListener {
            val mediaSource = MediaSource("/sdcard/trailer.mp4")
            mediaSource.usingMediaCodec = true
            videoPlayer = HYVideoPlayer(mediaSource)
            surface?.let {
                videoPlayer?.setSurfaceCreated(surface!!)
                videoPlayer?.setSurfaceChanged(surfaceWidth, surfaceHeight)
            }
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
    }

    override fun surfaceDoFrame() {
        videoPlayer?.doFrame()
    }
}