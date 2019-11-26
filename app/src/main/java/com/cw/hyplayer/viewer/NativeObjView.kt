package com.cw.hyplayer.viewer

import android.content.Context
import android.util.AttributeSet
import android.view.Choreographer
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

class NativeObjView : SurfaceView, SurfaceHolder.Callback, Choreographer.FrameCallback {

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
        nativeObjViewCreated(holder!!.surface)
        active = true
        Choreographer.getInstance().postFrameCallback(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        nativeObjViewChanged(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        active = false
        Choreographer.getInstance().removeFrameCallback(this)
        nativeObjViewDestroyed()
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (active) {
            nativeObjViewDoFrame()
            Choreographer.getInstance().postFrameCallback(this)
        }
    }

    private external fun nativeObjViewCreated(surface: Surface)
    private external fun nativeObjViewChanged(width: Int, height: Int)
    private external fun nativeObjViewDestroyed()
    private external fun nativeObjViewDoFrame()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

}