cmake_minimum_required(VERSION 3.4.1)

find_library(log-lib log)
find_library(android-lib android)

add_library(native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        src/main/cpp/base/Looper.cpp
        src/main/cpp/base/AVPacketData.cpp
        src/main/cpp/base/AVFrameData.cpp

        src/main/cpp/demux/IDemux.cpp
        src/main/cpp/demux/FFDemux.cpp
        src/main/cpp/demux/DemuxLooper.cpp
        src/main/cpp/demux/circle_av_packet_queue.cpp

        src/main/cpp/decode/IDecode.cpp
        src/main/cpp/decode/FFDecode.cpp
        src/main/cpp/decode/DecodeLooper.cpp
        src/main/cpp/decode/circle_av_frame_queue.cpp

        src/main/cpp/audio/IAudioPlayer.cpp
        src/main/cpp/audio/SLAudioPlayer.cpp
        src/main/cpp/audio/SLPlayerLooper.cpp

        src/main/cpp/player/AudioPlayer.cpp

        src/main/cpp/egl/egl_core.cpp
        src/main/cpp/egl/base_surface.cpp
        src/main/cpp/egl/window_surface.cpp
        src/main/cpp/egl/offscreen_surface.cpp

        src/main/cpp/template/TemplateLooper.cpp
        src/main/cpp/template/TemplateRenderer.cpp
        )

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


include_directories(
        ${JNI_LIBS_DIR}/includes
        ${JNI_LINS_DIR}/includes/libjpeg
        ${JNI_LINS_DIR}/includes/libpng16
        ${JNI_LINS_DIR}/includes/libgif
)

target_link_libraries(native-lib
        avutil swresample swscale avcodec avformat avfilter
        ${log-lib} ${android-lib}
        android EGL GLESv3 jnigraphics OpenSLES

        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libjpeg.a
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libpng16.a
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libgif.a
        )
