#version 420 core

uniform sampler2D tex;
uniform sampler2D normalMap;
uniform float ambient;
uniform float specular;
uniform mat4 normalMatrix;

in vec2 v_texCoord;
in vec3 v_color;

layout(location = 0) out vec3 color0;
layout(location = 1) out vec4 color1;

void main() {

    vec4 color = texture(tex, v_texCoord);
    if (color.a < 0.01 || (color.r > 0.1 && color.g > 0.1 && color.b > 0.1))
        discard;

    vec3 normal = (normalMatrix*vec4(texture(normalMap, v_texCoord).xyz, 1.0)).xyz;
    float p = sqrt(normal.z*8+8);
    vec2 encodedNormal = normal.xy/p;

    color0 = v_color;
    color1 = vec4(encodedNormal, ambient, specular);
}
