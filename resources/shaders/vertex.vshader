#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coordinates;

out vec2 v_texture_coordinates;

uniform mat4 u_MVP; // model view projection matrix

void main()
{
    gl_Position = u_MVP * position;
    v_texture_coordinates = texture_coordinates;
}
