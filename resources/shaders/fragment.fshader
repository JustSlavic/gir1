#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_color;
// in vec2 v_texture_coordinates;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    // vec4 texture_color = texture(u_Texture, v_texture_coordinates);
    // color = texture_color;
    color = v_color;
}
