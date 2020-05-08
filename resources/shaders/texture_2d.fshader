#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texture_coordinates;

uniform sampler2D u_Texture_1;

void main() {
    vec4 texture_color = texture(u_Texture_1, v_texture_coordinates);
    color = texture_color;
}
