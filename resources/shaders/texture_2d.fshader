#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texture_coordinates;
in vec3 v_fragment_position;
in vec3 v_normal;

uniform sampler2D u_Texture_1;
uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform vec3 u_view_position;

void main() {
    float ambient_strength = 0.1;
    vec3 ambient_light = ambient_strength * u_light_color;

    vec3 normal = normalize(v_normal);
    vec3 light_direction = normalize(u_light_position - v_fragment_position);
    float diffuse = max(dot(normal, light_direction), 0.0);
    vec3 diffuse_light = diffuse * u_light_color;

    float specular_strength = 0.5;
    int shininess = 256;
    vec3 view_direction = normalize(u_view_position - v_fragment_position);
    vec3 reflected_direction = reflect(-light_direction, normal);
    float specular = pow(max(dot(view_direction, reflected_direction), 0.0), shininess);
    vec3 specular_light = specular_strength * specular * u_light_color;

    vec3 result_light = ambient_light + diffuse_light + specular_light;

    vec4 texture_color = texture(u_Texture_1, v_texture_coordinates);
    color = texture_color * vec4(result_light, 1.0);
}
