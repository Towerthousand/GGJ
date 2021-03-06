#version 420 core //4.2

layout (lines) in;
layout (triangle_strip, max_vertices = 8) out;

uniform mat4  MVP;
uniform float verTrail;
uniform float horTrail;
uniform vec2  side;

in vec3 g_color[];

out vec2 v_texCoord;
out vec3 v_color;

int   inidx[8] = {0, 1, 0, 1, 0, 1, 0, 1};
float texY[8]  = {1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};
float zdisp[8] = {0.01f, 0.01f, 0.01f, 0.01f, -1.01f, -1.01f, -1.01f, -1.01f};

void main() {

    vec4 pos;

    for (int i = 0; i < 8; i++) {
        pos = vec4(side.x, side.y, 0, 0)*-0.01+gl_in[inidx[i]].gl_Position + texY[i]*vec4(side.x, side.y, 0.0f, 0.0f) + vec4(0.0f, 0.0f, zdisp[i], 0.0f);
        gl_Position = MVP*pos;
        v_texCoord = vec2(horTrail*pos.x/2 + verTrail*pos.y/2, texY[i]/2);
        v_color = g_color[0];
        EmitVertex();
    }

    EndPrimitive();

}
