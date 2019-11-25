package com.cw.hyplayer.encode.audio

import android.Manifest
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.cw.hyplayer.R
import kotlinx.android.synthetic.main.activity_audio_recoder.*
import java.io.File

class AudioEncoderActivity : AppCompatActivity() {

    private var audioEncoder: AudioEncoder? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_audio_recoder)

        init.setOnClickListener {
            if (Build.VERSION.SDK_INT >= 23 && ContextCompat.checkSelfPermission(
                    this,
                    Manifest.permission.RECORD_AUDIO
                ) == PackageManager.PERMISSION_GRANTED
            ) {
                initAudioEncoder()
            } else {
                ActivityCompat.requestPermissions(
                    this,
                    arrayOf(Manifest.permission.RECORD_AUDIO),
                    AUDIO_REQUEST_CODE
                )
            }
        }
        start.setOnClickListener {
            audioEncoder?.start()
        }
        release.setOnClickListener {
            audioEncoder?.release()
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        if (requestCode == AUDIO_REQUEST_CODE && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            initAudioEncoder()
        } else {
            Toast.makeText(this, "need audio record permission", Toast.LENGTH_LONG).show()
        }
    }

    private fun initAudioEncoder() {
        val file = File("sdcard/trailer_test.mp3")
        if (file.exists()) {
            file.delete()
            file.createNewFile()
        }
        audioEncoder = AudioEncoder("sdcard/trailer_test.mp3")
    }

    companion object {
        private const val AUDIO_REQUEST_CODE = 10001
    }
}