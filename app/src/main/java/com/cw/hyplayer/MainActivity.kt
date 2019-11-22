package com.cw.hyplayer

import android.Manifest
import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.ServiceConnection
import android.content.pm.PackageManager
import android.os.*
import androidx.appcompat.app.AppCompatActivity
import android.util.Log
import android.widget.Button
import android.widget.SeekBar
import android.widget.Toast
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.cw.hyplayer.audio.AudioService
import kotlinx.android.synthetic.main.activity_main.*
import java.util.*

class MainActivity : AppCompatActivity() {

    private lateinit var timer: Timer
    private lateinit var handler: Handler
    private var settable = true
    private var audioBinder: AudioService.AudioBinder? = null
    private var audioConn = AudioConn()
    private var isLoop = false

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
            audioBinder?.callCreate()
        }

        release.setOnClickListener {
            audioBinder?.callRelease()
        }

        start.setOnClickListener {
            audioBinder?.callStart()
        }
        pause.setOnClickListener {
            audioBinder?.callPause()
        }

        (loop as Button).text = "Loop $isLoop"
        loop.setOnClickListener {
            isLoop = !isLoop
            audioBinder?.callSetLoop(isLoop)
            (it as Button).text = "Loop $isLoop"
        }

        next.setOnClickListener {
            audioBinder?.callNext()
        }

        seek_bar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {

            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
                settable = false
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                if (seekBar == null || audioBinder == null) {
                    return
                }
                val pos = audioBinder!!.callTotalDuration() * seekBar.progress / seekBar.max
                Log.d("seek ", "$pos")
                audioBinder!!.callSeek(pos)
                settable = true
            }

        })

        volume_seek_bar.progress = 100
        volume_seek_bar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {

            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {

            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                seekBar?.let {
                    audioBinder!!.callSetVolume(it.progress)
                }
            }
        })

        handler = Handler(Looper.getMainLooper())
        timer = Timer()
        timer.scheduleAtFixedRate(object : TimerTask() {
            override fun run() {
                handler.post {
                    if (audioBinder != null && settable && audioBinder!!.callTotalDuration() > 0 && audioBinder!!.callCurrentTime() > 0) {
                        seek_bar.progress =
                            (audioBinder!!.callCurrentTime() * 100 / audioBinder!!.callTotalDuration()).toInt()
                    }
                }
            }
        }, 0, 500)


        val intent = Intent(this, AudioService::class.java)
        startForegroundService(intent)
        bindService(intent, audioConn, Context.BIND_AUTO_CREATE)
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
        unbindService(audioConn)
    }

    inner class AudioConn : ServiceConnection {

        override fun onServiceDisconnected(name: ComponentName?) {
            audioBinder = null
        }

        override fun onServiceConnected(name: ComponentName?, service: IBinder?) {
            audioBinder = service as AudioService.AudioBinder?
        }

    }

    companion object {
        private const val PERMISSION_CODE = 10001
    }
}
