#version 330 core

layout(location = 0) in vec3 position;

out vec3 v_texture_coordinates;

uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * vec4(position, 1.0);
    v_texture_coordinates = position;
}