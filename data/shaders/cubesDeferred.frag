#version 420

uniform sampler2D diffuseTex;
uniform sampler2D normalsTex;
uniform float ambient;
uniform float specular;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_tangent;

layout(location = 0) out vec3 color0;
layout(location = 1) out vec4 color1;

void main(void) {

        vec3 normal = normalize(v_normal);
        vec3 tangent = normalize(v_tangent);
        vec3 bitangent = normalize(cross(normal, tangent));

        vec3 mapnor = texture(normalsTex, v_texCoord).xyz;
        normal = (vec4(mat3(tangent, bitangent, normal)*mapnor, 0.0)).xyz;

        float p = sqrt(normal.z*8+8);
        vec2 encodedNormal = normal.xy/p;

        color0 = texture(diffuseTex, v_texCoord).xyz;
        color1 = vec4(encodedNormal, ambient, specular);
}
