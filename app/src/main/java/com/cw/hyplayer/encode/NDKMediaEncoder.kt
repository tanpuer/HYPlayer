package com.cw.hyplayer.encode

import java.io.File

class NDKMediaEncoder {

    fun start(width: Int, height: Int, bitRate: Int, outputFile: File) {
        nativeInit(width, height, bitRate, outputFile.absolutePath)
    }

    fun cancel() {
        nativeCancel()
    }

    private external fun nativeInit(width: Int, height: Int, bitRate: Int, path: String)
    private external fun nativeCancel()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}