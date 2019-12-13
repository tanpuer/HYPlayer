package com.cw.hyplayer.viewer

import android.content.Context
import android.util.AttributeSet
import android.util.Log
import android.view.*
import com.almeros.android.multitouch.MoveGestureDetector
import com.almeros.android.multitouch.RotateGestureDetector
import com.almeros.android.multitouch.ShoveGestureDetector

class NativeObjView : SurfaceView, SurfaceHolder.Callback, Choreographer.FrameCallback {

    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    private var active = false

    private var mScaleDetector: ScaleGestureDetector? = null
    private var mRotateDetector: RotateGestureDetector? = null
    private var mMoveDetector: MoveGestureDetector? = null
    private var mShoveDetector: ShoveGestureDetector? = null

    private val screenWidth = resources.displayMetrics.widthPixels
    private val screenHeight = resources.displayMetrics.heightPixels

    init {
        holder.addCallback(this)
        mScaleDetector = ScaleGestureDetector(context.applicationContext, ScaleListener(this))
        mRotateDetector = RotateGestureDetector(context.applicationContext, RotateListener(this))
        mMoveDetector = MoveGestureDetector(context.applicationContext, MoveListener(this))
        mShoveDetector = ShoveGestureDetector(context.applicationContext, ShoveListener(this))
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        mScaleDetector?.onTouchEvent(event)
        mRotateDetector?.onTouchEvent(event)
        mMoveDetector?.onTouchEvent(event)
        mShoveDetector?.onTouchEvent(event)
        return true
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        nativeObjViewCreated(holder!!.surface)
        active = true
        Choreographer.getInstance().postFrameCallback(this)
        nativeObjScrollAsync(
            (objScrollX * 100 / screenWidth * 4).toInt(),
            (objScrollY * 100 / screenHeight * 4).toInt()
        )
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        nativeObjViewChanged(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        active = false
        Choreographer.getInstance().removeFrameCallback(this)
        nativeObjViewDestroyed()
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (active) {
            nativeObjViewDoFrame()
            Choreographer.getInstance().postFrameCallback(this)
        }
    }

    private inner class ScaleListener(val gestureSurfaceView: NativeObjView) :
        ScaleGestureDetector.SimpleOnScaleGestureListener() {
        override fun onScale(detector: ScaleGestureDetector): Boolean {
//            Log.d(TAG, "scale ${detector.scaleFactor}")
            objScale *= detector.scaleFactor
            nativeObjScale(objScale)
            return true
        }
    }

    private var objScale = 1000000f
    private inner class RotateListener(val gestureSurfaceView: NativeObjView) :
        RotateGestureDetector.SimpleOnRotateGestureListener() {
        override fun onRotate(detector: RotateGestureDetector): Boolean {
//            Log.d(TAG, "rotate ${detector.rotationDegreesDelta}")

            return false
        }
    }

    private var objScrollX = 0f
    private var objScrollY = 0f

    private inner class MoveListener(val gestureSurfaceView: NativeObjView) :
        MoveGestureDetector.SimpleOnMoveGestureListener() {
        override fun onMove(detector: MoveGestureDetector): Boolean {
            objScrollX += detector.focusDelta.x
            objScrollY += detector.focusDelta.y
            nativeObjScroll(
                (objScrollX * 100 / screenWidth * 4).toInt(),
                (objScrollY * 100 / screenHeight * 4).toInt()
            )
            return true
        }
    }

    private inner class ShoveListener(val gestureSurfaceView: NativeObjView) :
        ShoveGestureDetector.SimpleOnShoveGestureListener() {
        override fun onShove(detector: ShoveGestureDetector): Boolean {
//            Log.d(TAG, "shove ${detector.shovePixelsDelta}")
            return false
        }
    }

    private external fun nativeObjViewCreated(surface: Surface)
    private external fun nativeObjViewChanged(width: Int, height: Int)
    private external fun nativeObjViewDestroyed()
    private external fun nativeObjViewDoFrame()
    private external fun nativeObjScroll(objScrollX: Int, objScrollY: Int)
    private external fun nativeObjScrollAsync(objScrollX: Int, objScrollY: Int)
    private external fun nativeObjScale(scale: Float)

    companion object {
        init {
            System.loadLibrary("native-lib")
        }

        private const val TAG = "NativeObjView"
    }

}