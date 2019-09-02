//
// Created by templechen on 2019-09-02.
//

#ifndef HYPLAYER_IMAGECREATOR_H
#define HYPLAYER_IMAGECREATOR_H


class ImageCreator {

public:

    struct ImageData {
        void *data;
        int width;
        int height;
    };

    ImageData* readImage(const char *path);

    void releaseImage(ImageData *imageData);

};


#endif //HYPLAYER_IMAGECREATOR_H
