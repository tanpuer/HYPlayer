//
// Created by DZSB-000848 on 2020/9/19.
//

#ifndef HYPLAYER_ALPHABASEFILTER_H
#define HYPLAYER_ALPHABASEFILTER_H


#include "TemplateBaseFilter.h"

class AlphaBaseFilter : public TemplateBaseFilter{

public:

    AlphaBaseFilter();

    virtual ~AlphaBaseFilter();

    virtual void updateMatrix();

};


#endif //HYPLAYER_ALPHABASEFILTER_H
