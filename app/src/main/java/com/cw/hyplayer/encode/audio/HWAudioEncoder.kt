package com.cw.hyplayer.encode.audio

import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaFormat
import android.util.Log

class HWAudioEncoder {

    companion object {
        private const val TAG = "HWAudioEncoder"
        private const val MIME_TYPE = "audio/mp4a-latm"
        private const val DEFAULT_SAMPLE_RATE = 44100
        private const val DEFAULT_CHANNELS = 2
        private const val DEFAULT_BIT_RATE = 128 * 1024
    }

    private var mediaCodec: MediaCodec? = null
    private val mBufferInfo = MediaCodec.BufferInfo()

    fun encodeStart(
        path: String,
        bufferSize: Int,
        sampleRateInHz: Int = DEFAULT_SAMPLE_RATE,
        channels: Int = DEFAULT_CHANNELS,
        bitRate: Int = DEFAULT_BIT_RATE
    ) {
        val audioFormat = MediaFormat.createAudioFormat(MIME_TYPE, sampleRateInHz, channels)
        audioFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitRate)
        audioFormat.setInteger(
            MediaFormat.KEY_AAC_PROFILE,
            MediaCodecInfo.CodecProfileLevel.AACObjectLC
        )
        audioFormat.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE, bufferSize)
        mediaCodec = MediaCodec.createEncoderByType(MIME_TYPE)
        if (mediaCodec == null) {
            Log.e(TAG, "can not create audio encoder $MIME_TYPE")
            return
        }
        mediaCodec?.apply {
            configure(audioFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE)
            start()
        }
    }

    fun encodeFrame(buffer: ByteArray, size: Int) {
        mediaCodec?.let {
            val inputIndex = it.dequeueInputBuffer(0)
            if (inputIndex == -1) {
                Log.e(TAG, "dequeueInputBuffer is -1")
            }
            val inputBuffer = it.getInputBuffer(inputIndex)
            inputBuffer?.apply {
                clear()
                put(buffer)
                mediaCodec?.queueInputBuffer(inputIndex, 0, size, 0, 0)
            }
            val outputIndex = it.dequeueOutputBuffer(mBufferInfo, 0)
            if (mBufferInfo.size > 0) {
                //callback
            }
            it.releaseOutputBuffer(outputIndex, false)
        }
        mediaCodec?.dequeueInputBuffer(0)
    }

    fun encodeEnd() {
        mediaCodec?.apply {
            stop()
            release()
        }
        mediaCodec = null
    }

}