#version 420

uniform mat4 MVP;

in vec3 a_position;
out vec2 vTexCoord;

void main() {
    vTexCoord = a_position.xy/2+0.5;
    gl_Position = MVP * vec4(a_position, 1.0);
}
