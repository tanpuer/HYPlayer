package com.cw.hyplayer.encode.audio

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.cw.hyplayer.R
import kotlinx.android.synthetic.main.activity_audio_capture.*

class AudioCaptureActivity : AppCompatActivity() {

    private lateinit var audioCapture: AudioCapture

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_audio_capture)

        init.setOnClickListener {
            audioCapture = AudioCapture()
        }
        start.setOnClickListener {
            audioCapture.startCapture()
        }
        stop.setOnClickListener {
            audioCapture.stopCapture()
        }
    }

}