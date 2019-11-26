//
// Created by templechen on 2019-11-26.
//

#ifndef HYPLAYER_OBJVIEWERFILTER_H
#define HYPLAYER_OBJVIEWERFILTER_H


class ObjViewerFilter {

public:

    ObjViewerFilter();

    void release();

    void doFrame();

    void setNativeWindowSize(int width, int height);

private:

};


#endif //HYPLAYER_OBJVIEWERFILTER_H
