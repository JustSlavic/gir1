#version 330 core

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texture_coordinates;

out vec2 v_texture_coordinates;
out vec3 v_world_coordinates;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * position;
    v_world_coordinates = vec3(u_model * position);
    v_texture_coordinates = texture_coordinates;
}
