package com.cw.hyplayer.flutter

import android.content.Context
import android.util.AttributeSet
import android.view.Choreographer
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

class FlutterView : SurfaceView, SurfaceHolder.Callback, Choreographer.FrameCallback {

    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    init {
        holder.addCallback(this)
    }

    private var active = false

    override fun surfaceCreated(holder: SurfaceHolder?) {
        active = true
        nativeFlutterViewCreated(holder!!.surface)
        Choreographer.getInstance().postFrameCallback(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        nativeFlutterViewChanged(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        active = false
        nativeFlutterViewDestroyed()
        Choreographer.getInstance().removeFrameCallback(this)
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (active) {
            nativeFlutterViewDoFrame(frameTimeNanos)
        }
        Choreographer.getInstance().postFrameCallback(this)
    }

    private external fun nativeFlutterViewCreated(surface: Surface)
    private external fun nativeFlutterViewChanged(width: Int, height: Int)
    private external fun nativeFlutterViewDestroyed()
    private external fun nativeFlutterViewDoFrame(frameTimeNanos: Long)

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

}