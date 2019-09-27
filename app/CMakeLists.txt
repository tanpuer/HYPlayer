cmake_minimum_required(VERSION 3.4.1)

find_library(log-lib log)
find_library(android-lib android)

#add_library(native-lib
#        SHARED
#        src/main/cpp/native-lib.cpp
#        src/main/cpp/base/Looper.cpp
#        src/main/cpp/base/AVPacketData.cpp
#        src/main/cpp/base/AVFrameData.cpp
#        src/main/cpp/base/gl_utils.cpp
#        src/main/cpp/base/utils.cpp
#
#        src/main/cpp/demux/IDemux.cpp
#        src/main/cpp/demux/FFDemux.cpp
#        src/main/cpp/demux/DemuxLooper.cpp
#        src/main/cpp/demux/circle_av_packet_queue.cpp
#
#        src/main/cpp/decode/IDecode.cpp
#        src/main/cpp/decode/FFDecode.cpp
#        src/main/cpp/decode/DecodeLooper.cpp
#        src/main/cpp/decode/circle_av_frame_queue.cpp
#
#        src/main/cpp/audio/IAudioPlayer.cpp
#        src/main/cpp/audio/SLAudioPlayer.cpp
#        src/main/cpp/audio/SLPlayerLooper.cpp
#
#        src/main/cpp/player/AudioPlayer.cpp
#
#        src/main/cpp/egl/egl_core.cpp
#        src/main/cpp/egl/base_surface.cpp
#        src/main/cpp/egl/window_surface.cpp
#        src/main/cpp/egl/offscreen_surface.cpp
#
#        src/main/cpp/template/TemplateLooper.cpp
#        src/main/cpp/template/TemplateRenderer.cpp
#        src/main/cpp/template/TemplateBaseFilter.cpp
#        src/main/cpp/template/TemplateFBOFilter.cpp
#
#        src/main/cpp/image/ImageCreator.cpp
#        src/main/cpp/image/IAVFrameCreator.cpp
#        src/main/cpp/image/GifCreator.cpp
#        src/main/cpp/image/VideoCreator.cpp
#
#        src/main/cpp/encode/MediaCodecEncoder.cpp
#        src/main/cpp/encode/NDKCodecEncoder.cpp
#        src/main/cpp/encode/JavaMediaEncoder.cpp
#        src/main/cpp/encode/VideoEncoder.cpp
#        src/main/cpp/encode/IVideoEncoder.cpp
#        src/main/cpp/encode/FFVideoEncoder.cpp
#        src/main/cpp/encode/FFmpegCodecEncoder.cpp
#        )

aux_source_directory(src/main/cpp                                                srcs)
aux_source_directory(src/main/cpp/audio                                         srcs)
aux_source_directory(src/main/cpp/base                                         srcs)
aux_source_directory(src/main/cpp/camera                                         srcs)
aux_source_directory(src/main/cpp/decode                                         srcs)
aux_source_directory(src/main/cpp/demux                                         srcs)
aux_source_directory(src/main/cpp/egl                                         srcs)
aux_source_directory(src/main/cpp/encode                                         srcs)
aux_source_directory(src/main/cpp/image                                         srcs)
aux_source_directory(src/main/cpp/player                                         srcs)
aux_source_directory(src/main/cpp/template                                         srcs)
aux_source_directory(src/main/cpp/flutter                                         srcs)
aux_source_directory(src/main/cpp/flutter/paint                                    srcs)
aux_source_directory(src/main/cpp/flutter/widget                                    srcs)
aux_source_directory(src/main/cpp/skia                                         srcs)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${srcs})

set(JNI_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs)

add_library(avutil
        SHARED
        IMPORTED)
set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavutil.so)

add_library(swresample
        SHARED
        IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libswresample.so)

add_library(swscale
        SHARED
        IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libswscale.so)

add_library(avcodec
        SHARED
        IMPORTED)
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavcodec.so)

add_library(avformat
        SHARED
        IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavformat.so)

add_library(avfilter
        SHARED
        IMPORTED)
set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavfilter.so)

add_library(libyuv
        SHARED
        IMPORTED)
set_target_properties(libyuv
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libyuv.so)


include_directories(
        ${JNI_LIBS_DIR}/includes
        ${JNI_LINS_DIR}/includes/libjpeg
        ${JNI_LINS_DIR}/includes/libpng16
        ${JNI_LINS_DIR}/includes/libgif
        ${JNI_LINS_DIR}/includes/libx264
        ${JNI_LINS_DIR}/includes/libyuv

        src/main/cpp/skia
        src/main/cpp/skia/android
        src/main/cpp/skia/atlastext
        src/main/cpp/skia/c
        src/main/cpp/skia/codec
        src/main/cpp/skia/config
        src/main/cpp/skia/core
        src/main/cpp/skia/effects
        src/main/cpp/skia/encode
        src/main/cpp/skia/gpu
        src/main/cpp/skia/pathops
        src/main/cpp/skia/ports
        src/main/cpp/skia/private
        src/main/cpp/skia/svg
        src/main/cpp/skia/utils
        src/main/cpp/skia/views

        src/main/cpp
        src/main/cpp/audio
        src/main/cpp/base
        src/main/cpp/camera
        src/main/cpp/decode
        src/main/cpp/demux
        src/main/cpp/egl
        src/main/cpp/encode
        src/main/cpp/image
        src/main/cpp/player
        src/main/cpp/template
        src/main/cpp/flutter
        src/main/cpp/flutter/paint
        src/main/cpp/flutter/widget
)

find_library(z-lib z)
find_library(camera-lib camera2ndk )
find_library(media-lib mediandk )

target_link_libraries(native-lib
        avutil swresample swscale avcodec avformat avfilter libyuv
        ${log-lib} ${android-lib}
        android EGL GLESv3 jnigraphics OpenSLES mediandk OpenMAXAL

        ${z-lib}
        ${camera2ndk}
        ${mediandk}

        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libjpeg.a
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libpng16.a
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libgif.a
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libx264.a

        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libskia.a
        )
