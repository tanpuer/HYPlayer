//
// Created by templechen on 2019-09-02.
//

#include <cstdio>
#include <cstring>
#include "ImageCreator.h"
#include "../base/native_log.h"

extern "C" {
#include "libpng16/png.h"
}

ImageCreator::ImageData *ImageCreator::readImage(const char *path) {
    auto *imageData = new ImageData();
    FILE *file = fopen(path, "rb");
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (png_ptr == nullptr) {
        ALOGD("readImage fail");
        fclose(file);
        return imageData;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        ALOGD("readImage fail");
        fclose(file);
        return imageData;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        ALOGD("readImage fail");
        fclose(file);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return imageData;
    }

    png_init_io(png_ptr, file);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);

    memcpy(imageData->data, row_pointers, 1);
    imageData->width = width;
    imageData->height = height;

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(file);

    ALOGD("readImage success %d %d", width, height);
    return imageData;
}

void ImageCreator::releaseImage(ImageCreator::ImageData *imageData) {
    delete imageData;
}
