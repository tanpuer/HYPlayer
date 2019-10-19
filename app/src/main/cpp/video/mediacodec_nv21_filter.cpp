//
// Created by templechen on 2019-10-17.
//

#include "mediacodec_nv21_filter.h"

static const char *fragNV21 = GET_STR(
        precision mediump float;
        varying vec2 vTextureCoord;
        uniform sampler2D uTextureY;
        uniform sampler2D uTextureUV;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(uTextureY,vTextureCoord).r;
            yuv.g = texture2D(uTextureUV,vTextureCoord).a - 0.5;
            yuv.b = texture2D(uTextureUV,vTextureCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);

void mediacodec_nv21_filter::initFragmentShader() {
    fragment_shader_string = fragNV21;
}
