package com.cw.hyplayer.encode

import android.os.Bundle
import android.os.Environment
import androidx.appcompat.app.AppCompatActivity
import com.cw.hyplayer.R
import kotlinx.android.synthetic.main.activity_encode.*
import java.io.File

class EncodeActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_encode)
        encode.setOnClickListener {
            val javaMediaEncoder = JavaMediaEncoder()
            var outputFile =
                File(Environment.getExternalStorageDirectory().absolutePath, "trailer_test.mp4")
            if (outputFile.exists()) {
                outputFile.delete()
                outputFile =
                    File(Environment.getExternalStorageDirectory().absolutePath, "trailer_test.mp4")
            }
            javaMediaEncoder.init(720, 1280, 2500000, outputFile)
        }

        ndk_encode.setOnClickListener {
            val ndkMediaEncoder = NDKMediaEncoder()
            var outputFile =
                File(Environment.getExternalStorageDirectory().absolutePath, "trailer_test.mp4")
            if (outputFile.exists()) {
                outputFile.delete()
                outputFile =
                    File(Environment.getExternalStorageDirectory().absolutePath, "trailer_test.mp4")
            }
            ndkMediaEncoder.start(720, 1280, 2500000, outputFile)
        }
    }

}