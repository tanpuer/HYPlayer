package com.cw.hyplayer.encode

import java.io.File

class FFmpegMediaEncoder {

    fun start(width: Int, height: Int, bitRate: Int, outputFile: File) {
        nativeInit(width, height, bitRate, outputFile.absolutePath)
    }

    private external fun nativeInit(width: Int, height: Int, bitRate: Int, path: String)

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}