////
//// Created by templechen on 2019-11-27.
////
//
//#include <base/gl_utils.h>
//#include <assert.h>
//#include "ObjViewer2DTextureFilter.h"
//#include "vector"
//
//static const char *VERTEX_SHADER = GET_STR(
//        attribute highp vec3 myVertex;
//        attribute highp vec3 myNormal;
//        attribute mediump vec2 myUV;
//        attribute mediump vec4 myBone;
//        varying mediump vec2 texCoord;
//        varying lowp vec4 diffuseLight;
//        varying mediump vec3 position;
//        varying mediump vec3 normal;
//        uniform highp mat4 uMVMatrix;
//        uniform highp mat4 uPMatrix;
//        uniform highp vec3 vLight0;
//        uniform lowp vec4 vMaterialDiffuse;
//        uniform lowp vec3 vMaterialAmbient;
//        uniform lowp vec4 vMaterialSpecular;
//        void main(void) {
//            highp vec4 p = vec4(myVertex,1);
//            gl_Position = uPMatrix * p;
//            texCoord = myUV;
//            highp vec3 worldNormal = vec3(mat3(uMVMatrix[0].xyz, uMVMatrix[1].xyz, uMVMatrix[2].xyz) * myNormal);
//            highp vec3 ecPosition = p.xyz;
//            diffuseLight = dot( worldNormal, normalize(-vLight0+ecPosition)) * vec4(1.0, 1.0, 1.0, 1.0);
//            normal = worldNormal;
//            position = ecPosition;
//        }
//);
//
//static const char *FRAGMENT_SHADER = GET_STR(
//        uniform lowp vec3 vMaterialAmbient;
//        uniform lowp vec4 vMaterialSpecular;
//        uniform sampler2D samplerObj;
//        varying mediump vec2 texCoord;
//        varying lowp vec4 diffuseLight;
//        uniform highp vec3   vLight0;
//        varying mediump vec3 position;
//        varying mediump vec3 normal;
//        void main() {
//            mediump vec3 halfVector = normalize(-vLight0 + position);
//            mediump float NdotH = max(dot(normalize(normal), halfVector), 0.0);
//            mediump float fPower = vMaterialSpecular.w;
//            mediump float specular = pow(NdotH, fPower);
//            lowp vec4 colorSpecular = vec4( vMaterialSpecular.xyz * specular, 1 );
//            // increase ambient light to brighten the teapot :-)
//            gl_FragColor = diffuseLight * texture2D(samplerObj, texCoord) +
//            2.0f * vec4(vMaterialAmbient.xyz, 1.0f) + colorSpecular;
//        }
//);
//
//void ObjViewer2DTextureFilter::initShaders() {
//    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER);
//    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
//}
//
//void ObjViewer2DTextureFilter::release() {
//    ObjViewerFilter::release();
//}
//
//void ObjViewer2DTextureFilter::init() {
//    ObjViewerFilter::init();
//
//    glGenBuffers(1, &texVBO);
//    assert(texVBO != GL_INVALID_VALUE);
//    /*
//     * Loading Texture coord directly from data declared in model file
//     *   teapot.inl
//     * which is 3 floats/vertex.
//     */
//    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
//
//    std::vector<float> coords;
//    for (int32_t idx = 0; idx < numVertices; idx++) {
//        coords.push_back(teapotTexCoords[3 * idx] / 2);
//        coords.push_back(teapotTexCoords[3* idx + 1] / 2);
//    }
//    glBufferData(GL_ARRAY_BUFFER,
//                 2 * sizeof(float) * numVertices,
//                 coords.data(), GL_STATIC_DRAW);
//
//    glVertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, GL_FALSE,
//                          2 * sizeof(float),
//                          BUFFER_OFFSET(0));
//    glEnableVertexAttribArray(ATTRIB_UV);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    texture2D = new Texture2D("sdcard/android_pie.bmp");
//    texture2D->create();
//    GLint sampler = glGetUniformLocation(shaderProgram->program,
//                                         "samplerObj");
//    glUniform1i(sampler, 0);
//    texture2D->bindTexture();
//}
//
