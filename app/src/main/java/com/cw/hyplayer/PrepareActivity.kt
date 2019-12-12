package com.cw.hyplayer

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.cw.hyplayer.encode.audio.AudioEncoderActivity
import com.cw.hyplayer.viewer.ObjViewerActivity
import kotlinx.android.synthetic.main.activity_prepare.*

class PrepareActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_prepare)
        audio_player.setOnClickListener {
            val intent = Intent(this, MainActivity::class.java)
            startActivity(intent)
        }

        audio_recoder.setOnClickListener {
            val intent = Intent(this, AudioEncoderActivity::class.java)
            startActivity(intent)
        }

        obj_loader.setOnClickListener {
            val intent = Intent(this, ObjViewerActivity::class.java)
            startActivity(intent)
        }
    }
}