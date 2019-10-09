//
// Created by templechen on 2019-09-26.
//

#include "NDKCamera.h"

void NDKCamera::openCamera(ACameraDevice_request_template templateId) {
    ACameraIdList *cameraIdList = nullptr;
    ACameraMetadata *cameraMetadata = nullptr;

    const char *selectedCameraId = nullptr;
    camera_status_t camera_status = ACAMERA_OK;
    ACameraManager *cameraManager = ACameraManager_create();

    camera_status = ACameraManager_getCameraIdList(cameraManager, &cameraIdList);
    if (camera_status != ACAMERA_OK) {
        ALOGE("Failed to get camera id list (reason: %d)\n", camera_status);
        return;
    }

    if (cameraIdList->numCameras < 1) {
        ALOGE("No camera device detected.\n");
        return;
    }

    selectedCameraId = cameraIdList->cameraIds[0];

    ALOGE("Trying to open Camera2 (id: %s, num of camera : %d)\n", selectedCameraId,
          cameraIdList->numCameras);

    camera_status = ACameraManager_getCameraCharacteristics(cameraManager, selectedCameraId,
                                                            &cameraMetadata);

    if (camera_status != ACAMERA_OK) {
        ALOGE("Failed to get camera meta data of ID:%s\n", selectedCameraId);
    }

    deviceStateCallbacks.onDisconnected = camera_device_on_disconnected;
    deviceStateCallbacks.onError = camera_device_on_error;

    camera_status = ACameraManager_openCamera(cameraManager, selectedCameraId,
                                              &deviceStateCallbacks, &cameraDevice);

    if (camera_status != ACAMERA_OK) {
        ALOGE("Failed to open camera device (id: %s)\n", selectedCameraId);
    }

    camera_status = ACameraDevice_createCaptureRequest(cameraDevice, templateId,
                                                       &captureRequest);

    if (camera_status != ACAMERA_OK) {
        ALOGE("Failed to create preview capture request (id: %s)\n", selectedCameraId);
    }

    ACaptureSessionOutputContainer_create(&captureSessionOutputContainer);

    captureSessionStateCallbacks.onReady = capture_session_on_ready;
    captureSessionStateCallbacks.onActive = capture_session_on_active;
    captureSessionStateCallbacks.onClosed = capture_session_on_closed;

    ACameraMetadata_free(cameraMetadata);
    ACameraManager_deleteCameraIdList(cameraIdList);
    ACameraManager_delete(cameraManager);
}

void NDKCamera::closeCamera() {
    camera_status_t camera_status = ACAMERA_OK;

    if (captureRequest != nullptr) {
        ACaptureRequest_free(captureRequest);
        captureRequest = nullptr;
    }

    if (cameraOutputTarget != nullptr) {
        ACameraOutputTarget_free(cameraOutputTarget);
        cameraOutputTarget = nullptr;
    }

    if (cameraDevice != nullptr) {
        camera_status = ACameraDevice_close(cameraDevice);

        if (camera_status != ACAMERA_OK) {
            ALOGE("Failed to close CameraDevice.\n");
        }
        cameraDevice = nullptr;
    }

    if (sessionOutput != nullptr) {
        ACaptureSessionOutput_free(sessionOutput);
        sessionOutput = nullptr;
    }

    if (captureSessionOutputContainer != nullptr) {
        ACaptureSessionOutputContainer_free(captureSessionOutputContainer);
        captureSessionOutputContainer = nullptr;
    }

    ALOGE("Close Camera\n");
}

void NDKCamera::startPreview(ANativeWindow *nativeWindow) {
    this->nativeWindow = nativeWindow;

    openCamera(TEMPLATE_PREVIEW);

    ACameraOutputTarget_create(nativeWindow, &cameraOutputTarget);
    ACaptureRequest_addTarget(captureRequest, cameraOutputTarget);

    ACaptureSessionOutput_create(nativeWindow, &sessionOutput);
    ACaptureSessionOutputContainer_add(captureSessionOutputContainer, sessionOutput);

    ACameraDevice_createCaptureSession(cameraDevice, captureSessionOutputContainer,
                                       &captureSessionStateCallbacks, &captureSession);

    ACameraCaptureSession_setRepeatingRequest(captureSession, nullptr, 1, &captureRequest, nullptr);
}

void NDKCamera::stopPreview() {
    closeCamera();
    if (nativeWindow != nullptr) {
        ANativeWindow_release(nativeWindow);
        nativeWindow = nullptr;
    }
}

void NDKCamera::camera_device_on_disconnected(void *context, ACameraDevice *device) {

}

void NDKCamera::camera_device_on_error(void *context, ACameraDevice *device, int error) {

}

void NDKCamera::capture_session_on_ready(void *context, ACameraCaptureSession *session) {

}

void NDKCamera::capture_session_on_active(void *context, ACameraCaptureSession *session) {

}

void NDKCamera::capture_session_on_closed(void *context, ACameraCaptureSession *session) {

}
