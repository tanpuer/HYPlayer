package com.cw.hyplayer.encode

import android.view.Surface
import java.io.File

class JavaMediaEncoder {

    private lateinit var videoEncoder: VideoEncoder

    fun init(width: Int, height: Int, bitRate: Int, outputFile: File) {
        videoEncoder = VideoEncoder(width, height, bitRate, outputFile)
        nativeInit(videoEncoder.mInputSurface, this)
    }

    fun release() {
        videoEncoder.release()
    }

    fun drainEncoderWithNoTimeOut(endOfStream: Boolean) {
        videoEncoder.drainEncoderWithNoTimeOut(endOfStream)
    }

    fun cancel(){
        nativeCancel()
    }

    private external fun nativeInit(surface: Surface, javaMediaEncoder: JavaMediaEncoder)
    private external fun nativeCancel()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}