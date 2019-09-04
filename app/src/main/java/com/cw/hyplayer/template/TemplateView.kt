package com.cw.hyplayer.template

import android.content.Context
import android.util.AttributeSet
import android.view.Choreographer
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

class TemplateView : SurfaceView, SurfaceHolder.Callback, Choreographer.FrameCallback{

    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    private var active = false

    init {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        active = true
        nativeTemplateViewCreated(holder!!.surface)
        Choreographer.getInstance().postFrameCallback(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        nativeTemplateViewChanged(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        active = false
        nativeTemplateViewDestroyed()
        Choreographer.getInstance().removeFrameCallback(this)
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (active) {
            nativeTemplateDoFrame(frameTimeNanos)
        }
        Choreographer.getInstance().postFrameCallback(this)
    }

    private external fun nativeTemplateViewCreated(surface: Surface)
    private external fun nativeTemplateViewChanged(width: Int, height: Int)
    private external fun nativeTemplateViewDestroyed()
    private external fun nativeTemplateDoFrame(frameTimeNanos: Long)

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

}