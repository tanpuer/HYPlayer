package com.cw.hyplayer.camera

import android.content.Context
import android.os.Environment
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import java.io.File

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

    private val camerav1 = CameraV1(context, this)
    private var currentMillSecond = -1L

    override fun surfaceCreated(holder: SurfaceHolder?) {
        var outputFile =
            File(Environment.getExternalStorageDirectory().absolutePath, "trailer_test.mp4")
        if (outputFile.exists()) {
            outputFile.delete()
            outputFile =
                File(Environment.getExternalStorageDirectory().absolutePath, "trailer_test.mp4")
        }

        nativeCameraCreated(
            holder!!.surface,
            display.mode.physicalWidth,
            display.mode.physicalHeight
        )
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

    fun encodeCameraData(data: ByteArray, width: Int, height: Int) {
        if (currentMillSecond < 0) {
            currentMillSecond = System.currentTimeMillis()
            nativeEncodeCameraData(data, width, height, 0)
        } else {
            nativeEncodeCameraData(data, width, height, System.currentTimeMillis() - currentMillSecond)
        }
    }

    private external fun nativeCameraCreated(surface: Surface, windowWidth: Int, windowHeight: Int)
    private external fun nativeCameraChanged(width: Int, height: Int)
    private external fun nativeCameraDestroyed()
    private external fun nativeEncodeCameraData(data: ByteArray, width: Int, height: Int, pts: Long)

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}