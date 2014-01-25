#version 420 core //4.2

layout (lines) in;
layout (triangle_strip, max_vertices = 8) out;

uniform mat4  MVP;
uniform float verTrail;
uniform float horTrail;
uniform vec2  side;

out vec2 v_texCoord;

int   inidx[8] = {0, 1, 0, 1, 0, 1, 0, 1};
float texY[8]  = {1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};
float zdisp[8] = {0.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f, -1.0f};

void main() {

    vec4 pos;

    for (int i = 0; i < 8; i++) {
        pos = gl_in[inidx[i]].gl_Position + texY[i]*vec4(side.x, side.y, 0.0f, 0.0f) + vec4(0.0f, 0.0f, zdisp[i], 0.0f);
        gl_Position = MVP*pos;
        v_texCoord = vec2(horTrail*pos.x + verTrail*pos.y, texY[i]);
        EmitVertex();
    }

    EndPrimitive();

}
