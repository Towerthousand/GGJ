#version 420 core //4.2

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4  MVP;
uniform float verTrail;
uniform float horTrail;
uniform vec2  side;

out vec2 v_texCoord;

void main() {

    vec4 pos;

    pos = gl_in[0].gl_Position;
    gl_Position = MVP*pos;
    v_texCoord  = vec2(horTrail*pos.x + verTrail*pos.y, 0);
    EmitVertex();

    pos = gl_in[0].gl_Position + vec4(side.x, side.y, 0.0, 1.0);
    gl_Position = MVP*pos;
    v_texCoord  = vec2(horTrail*pos.x + verTrail*pos.y, 1);
    EmitVertex();

    pos = gl_in[1].gl_Position + vec4(side.x, side.y, 0.0, 1.0);
    v_texCoord  = vec2(horTrail*pos.x + verTrail*pos.y, 1);
    gl_Position = MVP*pos;
    EmitVertex();

    pos = gl_in[1].gl_Position;
    gl_Position = MVP*pos;
    v_texCoord  = vec2(horTrail*pos.x + verTrail*pos.y, 0);
    EmitVertex();

}
