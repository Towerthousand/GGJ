#version 420 core

uniform sampler2D tex;

in vec2 v_texCoord;

out vec4 outColor;

void main() {

    vec4 color = texture(tex, v_texCoord);
    if (color.a < 0.01)
        discard;

    outColor = vec4(color.xyz, 2.0*color.a);

}
