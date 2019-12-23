//
// Created by templechen on 2019-12-20.
//

#ifndef HYPLAYER_NEWOBJFILTER_H
#define HYPLAYER_NEWOBJFILTER_H


#include "ObjViewerFilter.h"

struct vec3 {
    float v[3];

    vec3() {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
    }
};

typedef struct {
    GLuint vb_id;  // vertex buffer id
    int numTriangles;
    int material_id;
} DrawObject;

class NewObjFilter : public ObjViewerFilter {

public:

    std::vector<DrawObject> drawObjects;

    void loadObj() override;

    bool LoadObjAndConvert();

    void doFrame() override;

    void init() override;

    void initShaders() override;

private:

    float bmin[3];
    float bmax[3];

    bool hasSmoothingGroup(const tinyobj::shape_t &shape);

    void computeSmoothingNormals(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape,
                                 std::map<int, vec3> &smoothVertexNormals);

    std::map<std::string, int> texturesIdMap;

    float maxExtent = 1.0f;
};


#endif //HYPLAYER_NEWOBJFILTER_H
