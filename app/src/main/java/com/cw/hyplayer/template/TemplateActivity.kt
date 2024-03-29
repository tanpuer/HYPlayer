package com.cw.hyplayer.template

import android.graphics.Color
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.cw.hyplayer.R
import kotlinx.android.synthetic.main.activity_template.*

class TemplateActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_template)
        tvPoet.text = """
            北国风光，千里冰封，万里雪飘。
            望长城内外，惟馀莽莽；大河上下，顿失滔滔。
            山舞银蛇，原驰蜡象，欲与天公试比高。
            须晴日，看红妆素裹，分外妖娆。
            江山如此多娇，引无数英雄竞折腰。
            惜秦皇汉武，略输文采；唐宗宋祖，稍逊风骚。
            一代天骄，成吉思汗，只识弯弓射大雕。
            俱往矣，数风流人物，还看今朝。
        """

        template_container.setBackgroundColor(Color.TRANSPARENT)
    }
}