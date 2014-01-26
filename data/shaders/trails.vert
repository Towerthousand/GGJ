#version 420 core

in vec3 a_position;
in vec3 a_color;

out vec3 g_color;
void main () {
    g_color = a_color;
    gl_Position = vec4(a_position, 1.0);
}
