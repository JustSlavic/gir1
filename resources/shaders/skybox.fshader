#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_texture_coordinates;

uniform samplerCube u_Texture_skybox;

void main() {
    vec4 texture_color = texture(u_Texture_skybox, v_texture_coordinates);
    color = texture_color;
}
