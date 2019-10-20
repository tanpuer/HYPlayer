package com.cw.hyplayer.video

import android.os.Bundle
import android.view.Surface
import android.widget.SeekBar
import androidx.appcompat.app.AppCompatActivity
import com.cw.hyplayer.R
import com.cw.hyplayer.audio.MediaSource
import kotlinx.android.synthetic.main.activity_video.*

class HYVideoActivity : AppCompatActivity(), IVideoViewCallback {

    private var videoPlayer: HYVideoPlayer? = null
    private var surface: Surface? = null
    private var surfaceWidth = 0
    private var surfaceHeight = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_video)

        video_view.videoViewCallback = this

        create_soft_decoder.setOnClickListener {
            val mediaSource = MediaSource("/sdcard/trailer111.mp4")
            videoPlayer = HYVideoPlayer(mediaSource)
            surface?.let {
                videoPlayer?.setSurfaceCreated(surface!!)
                videoPlayer?.setSurfaceChanged(surfaceWidth, surfaceHeight)
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

            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {

            }

        })
    }

    override fun surfaceCreated(surface: Surface) {
        this.surface = surface
    }

    override fun surfaceChanged(width: Int, height: Int) {
        surfaceWidth = width
        surfaceHeight = height
    }

    override fun surfaceDestroyed() {
        videoPlayer?.setSurfaceDestroyed()
    }

    override fun surfaceDoFrame() {
        videoPlayer?.doFrame()
    }
}