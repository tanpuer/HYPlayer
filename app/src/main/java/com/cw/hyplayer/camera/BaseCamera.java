package com.cw.hyplayer.camera;

import android.graphics.SurfaceTexture;

public abstract class BaseCamera {

    protected ICameraSizeListener iCameraSizeListener;

    public abstract boolean openCamera(int width, int height, int cameraId);

    public abstract void setSurfaceTexture(SurfaceTexture mSurfaceTexture);

    public abstract void startPreview();

    public abstract void stopPreview();

    public abstract void releaseCamera();

    public void setiCameraSizeListener(ICameraSizeListener iCameraSizeListener) {
        this.iCameraSizeListener = iCameraSizeListener;
    }
}
