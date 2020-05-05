#version 330 core

layout(location = 0) out vec4 color;

// in vec4 v_color;
in vec2 v_texture_coordinates;

uniform sampler2D u_Texture_1;
uniform sampler2D u_Texture_2;

void main()
{
    vec4 texture_color = mix(
        texture(u_Texture_1, v_texture_coordinates),
        texture(u_Texture_2, v_texture_coordinates),
        0.3);

    color = texture_color;
    // color = v_color;
}
