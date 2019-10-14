package com.cw.hyplayer.camera

import android.app.Activity
import android.content.Context
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.view.Choreographer
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

class NativeCameraView : SurfaceView, SurfaceHolder.Callback, Choreographer.FrameCallback {

    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    init {
        holder.addCallback(this)
        cameraV2 = CameraV2(context as Activity)
        cameraV2!!.openCamera(1080, 1920, 0)
    }

    private var active = false
    private var surfaceTexture: SurfaceTexture? = null
    private var cameraV2: CameraV2? = null

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
        cameraV2?.stopPreview()
        cameraV2?.releaseCamera()
        nativeCameraDestroyed()
        Choreographer.getInstance().removeFrameCallback(this)
        postDelayed({ surfaceTexture?.release() }, 1000)
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (active) {
            nativeCameraDoFrame()
        }
        Choreographer.getInstance().postFrameCallback(this)
    }

    fun createOESSurface(oesTextureId: Int) {
        surfaceTexture = SurfaceTexture(oesTextureId)
        cameraV2?.setSurfaceTexture(surfaceTexture)
        cameraV2?.startPreview()
    }

    fun update() {
        surfaceTexture?.updateTexImage()
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