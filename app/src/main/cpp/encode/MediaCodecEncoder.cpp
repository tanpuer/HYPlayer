//
// Created by templechen on 2019-09-05.
//

#include "MediaCodecEncoder.h"
#include "OffscreenRenderer.h"

MediaCodecEncoder::MediaCodecEncoder(jobject surface, jobject javaMediaEncoder, JavaVM *vm) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    this->surface = surface;
    this->javaMediaEncoder = javaMediaEncoder;
    this->vm = vm;
    pthread_create(&worker_thread, &attr, trampoline, this);
}

MediaCodecEncoder::~MediaCodecEncoder() {

}

void *MediaCodecEncoder::trampoline(void *p) {
    OffscreenRenderer *renderer = new OffscreenRenderer();
    renderer->templateCreated(720, 1280, ((MediaCodecEncoder *) p)->surface,
                              ((MediaCodecEncoder *) p)->javaMediaEncoder,
                              ((MediaCodecEncoder *) p)->vm);
    delete renderer;
    return nullptr;
}
