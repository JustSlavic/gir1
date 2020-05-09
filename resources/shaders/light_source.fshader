#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_light_color;

void main() {
    color = vec4(u_light_color, 1.0);
}
