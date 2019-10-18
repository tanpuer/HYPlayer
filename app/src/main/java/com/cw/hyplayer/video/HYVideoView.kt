package com.cw.hyplayer.video

import android.content.Context
import android.util.AttributeSet
import android.view.Choreographer
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.cw.hyplayer.audio.MediaSource

class HYVideoView : SurfaceView, SurfaceHolder.Callback, Choreographer.FrameCallback {

    private val videoPlayer: HYVideoPlayer
    private var active = false

    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    constructor(
        context: Context?,
        attrs: AttributeSet?,
        defStyleAttr: Int,
        defStyleRes: Int
    ) : super(context, attrs, defStyleAttr, defStyleRes)

    init {
        val mediaSource = MediaSource("/sdcard/trailer111.mp4")
        videoPlayer = HYVideoPlayer(mediaSource)
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        active = true
        videoPlayer.setSurfaceCreated(holder!!.surface)
        Choreographer.getInstance().postFrameCallback(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        videoPlayer.setSurfaceChanged(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        active = false
        Choreographer.getInstance().removeFrameCallback(this)
        videoPlayer.setSurfaceDestroyed()
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (active) {
            videoPlayer.doFrame()
        }
        Choreographer.getInstance().postFrameCallback(this)
    }
}