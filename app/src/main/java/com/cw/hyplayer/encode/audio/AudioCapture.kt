package com.cw.hyplayer.encode.audio

import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder
import android.util.Log
import java.lang.Exception
import kotlin.math.min

class AudioCapture {

    companion object {
        private const val TAG = "AudioCapture"
        private const val DEFAULT_SOURCE = MediaRecorder.AudioSource.MIC
        private const val DEFAULT_SAMPLE_RATE = 44100
        private const val DEFAULT_CHANNEL_CONFIG = AudioFormat.CHANNEL_IN_MONO
        private const val DEFAULT_AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT
    }

    private var mIsCaptureStarted = false
    private var isLoopExit = false

    private lateinit var mAudioRecord: AudioRecord
    private var minBufferSize = 0
    var audioCaptureListener: AudioCaptureListener? = null
    private lateinit var mCaptureThread: Thread

    fun startCapture() {
        startCapture(
            DEFAULT_SOURCE,
            DEFAULT_SAMPLE_RATE,
            DEFAULT_CHANNEL_CONFIG,
            DEFAULT_AUDIO_FORMAT
        )
    }

    fun startCapture(audioSource: Int, sampleRateInHz: Int, channelConfig: Int, audioFormat: Int) {
        if (mIsCaptureStarted) {
            Log.e(TAG, "audio capture already started")
            return
        }
        minBufferSize = AudioRecord.getMinBufferSize(sampleRateInHz, channelConfig, audioFormat)
        if (minBufferSize == AudioRecord.ERROR_BAD_VALUE) {
            Log.e(TAG, "invalid audio param")
            return
        }
        mAudioRecord =
            AudioRecord(audioSource, sampleRateInHz, channelConfig, audioFormat, minBufferSize)
        if (mAudioRecord.state == AudioRecord.STATE_UNINITIALIZED) {
            Log.e(TAG, "audio capture initialize failed")
            return
        }
        mAudioRecord.startRecording()
        isLoopExit = false
        mCaptureThread = Thread(AudioCaptureRunnable())
        mCaptureThread.start()
        mIsCaptureStarted = true
        Log.d(TAG, "audio capture start success")
    }

    fun stopCapture() {
        if (!mIsCaptureStarted) {
            Log.e(TAG, "audio capture already stop")
            return
        }
        isLoopExit = true
        try {
            mCaptureThread.interrupt()
            mCaptureThread.join()
        } catch (e: Exception) {
            e.printStackTrace()
        }
        if (mAudioRecord.state == AudioRecord.RECORDSTATE_RECORDING) {
            mAudioRecord.stop();
        }
        mAudioRecord.release()
        mIsCaptureStarted = false
        audioCaptureListener = null
        Log.d(TAG, "audio capture stop success")
    }

    interface AudioCaptureListener {
        fun onAudioFrameCapture(buffer: ByteArray)
    }

    private inner class AudioCaptureRunnable : Runnable {
        override fun run() {
            while (!isLoopExit) {
                val buffer = ByteArray(minBufferSize)
                val ret = mAudioRecord.read(buffer, 0, minBufferSize)
                when (ret) {
                    AudioRecord.ERROR_INVALID_OPERATION -> {
                        Log.d(TAG, "audioRecord ERROR_INVALID_OPERATION")
                    }
                    AudioRecord.ERROR_BAD_VALUE -> {
                        Log.d(TAG, "audioRecord ERROR_BAD_VALUE")
                    }
                    else -> {
                        audioCaptureListener?.onAudioFrameCapture(buffer)
                    }
                }
            }
        }

    }
}