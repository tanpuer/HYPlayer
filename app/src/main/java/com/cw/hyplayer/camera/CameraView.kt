package com.cw.hyplayer.camera

import android.content.Context
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

class CameraView : SurfaceView, SurfaceHolder.Callback {

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

    private val camerav1 = CameraV1()

    override fun surfaceCreated(holder: SurfaceHolder?) {
        nativeCameraCreated(holder!!.surface, display.mode.physicalWidth, display.mode.physicalHeight)
        camerav1.setContext(context)
//        camerav1.setPreviewView(this)
        camerav1.surfaceCreated(holder)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        nativeCameraChanged(width, height)
        camerav1.surfaceChanged(holder, format, width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        nativeCameraDestroyed()
        camerav1.surfaceDestroyed(holder)
    }

    private external fun nativeCameraCreated(surface: Surface, windowWidth: Int, windowHeight: Int)
    private external fun nativeCameraChanged(width: Int, height: Int)
    private external fun nativeCameraDestroyed()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}