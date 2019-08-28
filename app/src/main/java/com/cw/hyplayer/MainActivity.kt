package com.cw.hyplayer

import android.Manifest
import android.content.pm.PackageManager
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.widget.SeekBar
import android.widget.Toast
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.cw.hyplayer.audio.HYAudioPlayer
import com.cw.hyplayer.audio.MediaSource
import kotlinx.android.synthetic.main.activity_main.*
import java.util.*

class MainActivity : AppCompatActivity() {

    private var audioPlayer: HYAudioPlayer? = null
    private lateinit var timer: Timer
    private lateinit var handler: Handler
    private var settable = true

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        if (Build.VERSION.SDK_INT >= 21 && ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE),
                PERMISSION_CODE
            )
        }

        create.setOnClickListener {
            if (audioPlayer == null) {
                val mediaSource = MediaSource("/sdcard/test.mp3")
//                val mediaSource = MediaSource("/sdcard/trailer111.mp4")
                audioPlayer = HYAudioPlayer(mediaSource)
            }
        }

        release.setOnClickListener {
            audioPlayer?.release()
            audioPlayer = null
        }

        start.setOnClickListener {
            audioPlayer?.start()
        }
        pause.setOnClickListener {
            audioPlayer?.pause()
        }

        seek_bar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {

            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
                settable = false
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                if (seekBar == null || audioPlayer == null) {
                    return
                }
                val pos = audioPlayer!!.totalDuration() * seekBar.progress / seekBar.max
                Log.d("seek ", "$pos")
                audioPlayer!!.seek(pos)
                settable = true
            }

        })

        handler = Handler(Looper.getMainLooper())
        timer = Timer()
        timer.scheduleAtFixedRate(object : TimerTask() {
            override fun run() {
                handler.post {
                    if (audioPlayer != null && settable && audioPlayer!!.totalDuration() > 0) {
                        Log.d("111", "${audioPlayer!!.currentTime()} : ${audioPlayer!!.totalDuration()}")
                        seek_bar.progress = (audioPlayer!!.currentTime() * 100 / audioPlayer!!.totalDuration()).toInt()
                    }
                }
            }
        }, 0, 500)
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        if (requestCode == PERMISSION_CODE && grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
        } else {
            Toast.makeText(this, "must need write external storage permission!", Toast.LENGTH_LONG)
                .show()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        timer.cancel()
    }

    companion object {
        private const val PERMISSION_CODE = 10001
    }
}
