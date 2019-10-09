//
// Created by templechen on 2019-09-26.
//

#ifndef HYPLAYER_NDKCAMERA_H
#define HYPLAYER_NDKCAMERA_H

#include <android/native_window_jni.h>

#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraCaptureSession.h>
#include <base/native_log.h>

class NDKCamera {

public:

    void startPreview(ANativeWindow *nativeWindow);

    void openCamera(ACameraDevice_request_template templateId);

    void closeCamera();

    void stopPreview();

private:

    ANativeWindow *nativeWindow;
    ACameraDevice *cameraDevice;
    ACaptureRequest *captureRequest;
    ACameraOutputTarget *cameraOutputTarget;
    ACaptureSessionOutput *sessionOutput;
    ACaptureSessionOutputContainer *captureSessionOutputContainer;
    ACameraCaptureSession *captureSession;

    ACameraDevice_StateCallbacks deviceStateCallbacks;
    ACameraCaptureSession_stateCallbacks captureSessionStateCallbacks;

    static void camera_device_on_disconnected(void *context, ACameraDevice *device);

    static void camera_device_on_error(void *context, ACameraDevice *device, int error);

    static void capture_session_on_ready(void *context, ACameraCaptureSession *session);

    static void capture_session_on_active(void *context, ACameraCaptureSession *session);

    static void capture_session_on_closed(void *context, ACameraCaptureSession *session);

};


#endif //HYPLAYER_NDKCAMERA_H
