package com.cw.hyplayer.video

import android.content.Context
import android.util.AttributeSet
import android.view.Choreographer
import android.view.SurfaceHolder
import android.view.SurfaceView

class HYVideoView : SurfaceView, SurfaceHolder.Callback, Choreographer.FrameCallback {

    private var active = false
    var videoViewCallback: IVideoViewCallback? = null

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
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        active = true
        videoViewCallback?.surfaceCreated(holder!!.surface)
        Choreographer.getInstance().postFrameCallback(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        videoViewCallback?.surfaceChanged(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        active = false
        videoViewCallback?.surfaceDestroyed()
        Choreographer.getInstance().removeFrameCallback(this)
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (active) {
            videoViewCallback?.surfaceDoFrame()
        }
        Choreographer.getInstance().postFrameCallback(this)
    }
}