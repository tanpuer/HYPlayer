package com.cw.hyplayer.camera

import android.content.Context
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.view.Choreographer
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

class NativeCameraView : SurfaceView, SurfaceHolder.Callback, Choreographer.FrameCallback{

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
    private var surafaceTexture : SurfaceTexture? = null

    override fun surfaceCreated(holder: SurfaceHolder?) {
        active = true
        nativeCameraCreated(holder!!.surface, this)
        Choreographer.getInstance().postFrameCallback(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        nativeCameraChanged(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        active = false
        nativeCameraDestroyed()
        Choreographer.getInstance().removeFrameCallback(this)
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (active) {
            nativeCameraDoFrame()
        }
        Choreographer.getInstance().postFrameCallback(this)
    }

    fun createOESSurface(oesTextureId: Int): Surface {
        surafaceTexture = SurfaceTexture(oesTextureId)
        return Surface(surafaceTexture)
    }

    fun update() {
        surafaceTexture?.updateTexImage()
    }

    private external fun nativeCameraCreated(surface: Surface, nativeCameraView: NativeCameraView)
    private external fun nativeCameraChanged(width: Int, height: Int)
    private external fun nativeCameraDestroyed()
    private external fun nativeCameraDoFrame()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

}