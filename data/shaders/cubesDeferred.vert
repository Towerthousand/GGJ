#version 420

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texCoord;
in vec3 a_tangent;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_position;

void main(void) {
        v_texCoord  = a_texCoord;
        v_normal    = normalize(vec4(V*M*vec4(a_normal, 0.0)).xyz);
        v_tangent   = normalize(vec4(V*M*vec4(a_tangent, 0.0)).xyz);
        v_position  = a_position;
        gl_Position = MVP * vec4(a_position, 1.0);
}
