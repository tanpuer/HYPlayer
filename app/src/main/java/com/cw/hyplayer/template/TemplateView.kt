package com.cw.hyplayer.template

import android.content.Context
import android.graphics.Color
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.view.*

class TemplateView : TextureView, SurfaceHolder.Callback, Choreographer.FrameCallback,
    TextureView.SurfaceTextureListener {

    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    private var active = false

    init {
//        holder.addCallback(this)
//        setBackgroundColor(Color.TRANSPARENT)
//        setZOrderOnTop(true)
//        setZOrderMediaOverlay(true)

        surfaceTextureListener = this
        isOpaque = false
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

    override fun onSurfaceTextureAvailable(surface: SurfaceTexture?, width: Int, height: Int) {
        active = true
        nativeTemplateViewCreated(Surface(surface!!))
        Choreographer.getInstance().postFrameCallback(this)
        nativeTemplateViewChanged(width, height)
    }

    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture?, width: Int, height: Int) {
    }

    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture?): Boolean {
        active = false
        nativeTemplateViewDestroyed()
        Choreographer.getInstance().removeFrameCallback(this)
        return true
    }

    override fun onSurfaceTextureUpdated(surface: SurfaceTexture?) {
    }

}