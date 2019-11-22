package com.cw.hyplayer.camera;

import android.app.Activity;
import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.Image;
import android.media.ImageReader;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.util.Log;
import android.util.Size;
import android.util.SparseIntArray;
import android.view.Surface;

import androidx.annotation.NonNull;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class CameraV2 extends BaseCamera {

    private static final String TAG = "AndroidCamera";
    private static final SparseIntArray ORIENTATIONS = new SparseIntArray();
    private static final int CAMERA_WIDTH = 1920;
    private static final int CAMERA_HEIGHT = 1080;

    static {
        ORIENTATIONS.append(Surface.ROTATION_0, 0);
        ORIENTATIONS.append(Surface.ROTATION_90, 90);
        ORIENTATIONS.append(Surface.ROTATION_180, 180);
        ORIENTATIONS.append(Surface.ROTATION_270, 270);
    }

    private String cameraId = null;
    private CameraManager cameraManager;
    private Activity mActivity;
    private CameraDevice cameraDevice;
    private Size mPreviewSize;
    private HandlerThread cameraThread;
    private Handler cameraHandler;
    private SurfaceTexture mSurfaceTexture;
    private CaptureRequest.Builder mCaptureRequestBuilder;
    private CaptureRequest mCaptureRequest;
    private CameraCaptureSession mSession;

    private ImageReader mImageReader;
    private NativeCameraView nativeCameraView;

    private boolean isPreview = false;

    public CameraV2(Activity activity, NativeCameraView nativeCameraView) {
        this.mActivity = activity;
        this.nativeCameraView = nativeCameraView;
    }

    private void setCameraThread() {
        cameraThread = new HandlerThread("Camera Thread");
        cameraThread.start();
        cameraHandler = new Handler(cameraThread.getLooper());
    }

    public boolean openCamera(int width, int height, int cameraFacing) {
        setCameraThread();
        if (width == 0 || height == 0) {
            width = CAMERA_WIDTH;
            height = CAMERA_HEIGHT;
        }
        cameraManager = (CameraManager) mActivity.getSystemService(Context.CAMERA_SERVICE);
        try {
            for (String cameraID : cameraManager.getCameraIdList()) {
                CameraCharacteristics cameraCharacteristics = cameraManager.getCameraCharacteristics(cameraID);
                int facing = cameraCharacteristics.get(CameraCharacteristics.LENS_FACING).intValue();
                if (facing == cameraFacing) {
                    this.cameraId = cameraID;
                    StreamConfigurationMap map = cameraCharacteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
                    mPreviewSize = getOptimalSize(map.getOutputSizes(SurfaceTexture.class), width, height);
                    Log.d(TAG, "openCamera with cameraId :" + cameraId + ", preview-width: " + mPreviewSize.getWidth() + " height: " + mPreviewSize.getHeight());
                    if (iCameraSizeListener != null) {
                        iCameraSizeListener.onCameraSizeChanged(mPreviewSize.getHeight(), mPreviewSize.getWidth());
                    }
                    initImageReader();
                    openCameraV2();
                    return true;
                }
            }
        } catch (CameraAccessException e) {
            e.printStackTrace();
            return false;
        } catch (NullPointerException e) {
            e.printStackTrace();
        }
        return false;
    }

    private Size getOptimalSize(Size[] sizeMap, int width, int height) {
        List<Size> sizeList = new ArrayList<>();
        for (Size option : sizeMap) {
            Log.d(TAG, "getOptimalSize: " + option.getWidth() + " * " + option.getHeight());
            if (width == option.getWidth() && height == option.getHeight()) {
                return new Size(width, height);
            }
            if (width > height) {
                if (option.getWidth() >= width && option.getHeight() >= height) {
                    sizeList.add(option);
                }
            } else {
                if (option.getWidth() >= height && option.getHeight() >= width) {
                    sizeList.add(option);
                }
            }
        }
        if (sizeList.size() > 0) {
            return Collections.min(sizeList, new Comparator<Size>() {
                @Override
                public int compare(Size lhs, Size rhs) {
                    return Long.signum(lhs.getWidth() * lhs.getHeight() - rhs.getWidth() * rhs.getHeight());
                }
            });
        }
        return sizeMap[0];
    }

    private void openCameraV2() {
        if (cameraId == null) {
            return;
        }
        try {
            cameraManager.openCamera(cameraId, stateCallback, cameraHandler);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        }
    }

    private void initImageReader() {
        mImageReader = ImageReader.newInstance(
                mPreviewSize.getWidth(), // 宽度
                mPreviewSize.getHeight(), // 高度
                ImageFormat.YUV_420_888, // 图像格式
                2);
        mImageReader.setOnImageAvailableListener(new ImageReader.OnImageAvailableListener() {
            @Override
            public void onImageAvailable(ImageReader reader) {
                if (isPreview) {
                    Image img = reader.acquireNextImage();
                    Log.d(TAG, "onImageAvailable format: " + img.getFormat() + ", width: " + img.getWidth() + ", height: " + img.getHeight());
                    if (mImageReader.getSurface() == null) {
                        img.close();
                        return;
                    }
                    ByteBuffer buffer = img.getPlanes()[0].getBuffer();
                    byte[] data = new byte[buffer.remaining()];
                    buffer.get(data);
                    nativeCameraView.cameraFFEncodeFrame(data, img.getWidth(), img.getHeight());
                    img.close();
                }
            }
        }, cameraHandler);
        nativeCameraView.cameraFFEncodeStart(mPreviewSize.getWidth(), mPreviewSize.getHeight());
    }

    private final CameraDevice.StateCallback stateCallback = new CameraDevice.StateCallback() {
        @Override
        public void onOpened(@NonNull CameraDevice camera) {
            cameraDevice = camera;
            startPreview();
        }

        @Override
        public void onDisconnected(@NonNull CameraDevice camera) {
            cameraDevice = null;
        }

        @Override
        public void onError(@NonNull CameraDevice camera, int error) {
            stopPreview();
            releaseCamera();
            cameraDevice = null;
            Log.d(TAG, "onError: " + error);
        }
    };

    public void setSurfaceTexture(SurfaceTexture mSurfaceTexture) {
        this.mSurfaceTexture = mSurfaceTexture;
    }

    public void startPreview() {
        mSurfaceTexture.setDefaultBufferSize(mPreviewSize.getWidth(), mPreviewSize.getHeight());
        Surface surface = new Surface(mSurfaceTexture);
        try {
            mCaptureRequestBuilder = cameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
            mCaptureRequestBuilder.addTarget(surface);
            mCaptureRequestBuilder.addTarget(mImageReader.getSurface());
            cameraDevice.createCaptureSession(Arrays.asList(surface, mImageReader.getSurface()), new CameraCaptureSession.StateCallback() {
                @Override
                public void onConfigured(@NonNull CameraCaptureSession session) {
                    mSession = session;
                    mCaptureRequest = mCaptureRequestBuilder.build();
                    try {
                        // 自动对焦
                        mCaptureRequestBuilder.set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_VIDEO);
                        // 打开闪光灯
//                        mCaptureRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH);
                        mSession.setRepeatingRequest(mCaptureRequest, null, cameraHandler);
                    } catch (CameraAccessException e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onConfigureFailed(@NonNull CameraCaptureSession session) {

                }
            }, cameraHandler);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
        isPreview = true;
    }

    public void stopPreview() {
        isPreview = false;
        nativeCameraView.cameraFFEncodeEnd();
        try {
            mSession.stopRepeating();
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void releaseCamera() {
        if (mImageReader != null) {
            mImageReader.close();
            mImageReader = null;
        }
        if (mSession != null) {
            mSession.close();
            mSession = null;
        }
        if (cameraDevice != null) {
            cameraDevice.close();
            cameraDevice = null;
        }
        cameraThread.quitSafely();
        try {
            cameraThread.join();
            cameraThread = null;
            cameraHandler = null;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void setCameraSizeListener(ICameraSizeListener iCameraSizeListener) {
        this.iCameraSizeListener = iCameraSizeListener;
    }
}
