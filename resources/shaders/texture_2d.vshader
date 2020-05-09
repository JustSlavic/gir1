#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinates;

out vec2 v_texture_coordinates;
out vec3 v_fragment_position;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_normal_matrix;

void main() {
    gl_Position = u_projection * u_view * u_model * position;
    v_texture_coordinates = texture_coordinates;
    v_fragment_position = vec3(u_model * position);
    v_normal = mat3(u_normal_matrix) * normal;
}
