#version 420

uniform sampler2D diffuseTex;
uniform float ambient;
uniform float specular;
uniform int colorID;

in vec2 v_texCoord;
in vec3 v_normal;

layout(location = 0) out vec3 color0;
layout(location = 1) out vec4 color1;

void main(void) {
        vec3 color;
        switch (colorID) {
        case 0: color = vec3(1,1,1); break;
        case 1: color = vec3(1,0,0); break;
        case 2: color = vec3(0,1,0); break;
        case 3: color = vec3(0,0,1); break;
        }

        color0 = texture(diffuseTex, v_texCoord).xyz*color;
        vec3 normal = normalize(v_normal);
        float p = sqrt(normal.z*8+8);
        vec2 encodedNormal = normal.xy/p;
        color1 = vec4(encodedNormal, ambient, specular);
}
