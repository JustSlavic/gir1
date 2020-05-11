#version 330 core

struct Material {
    sampler2D diffuse;   // texture defines diffuse color
    sampler2D specular;  // specular map is also a texture
    float     shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;  // ambient intencity of a light source
    vec3 diffuse;  // color of the light
    vec3 specular; // shininess is usually vec3(1.0)
};

layout(location = 0) out vec4 color;

in vec2 v_texture_coordinates;
in vec3 v_fragment_position;
in vec3 v_normal;

uniform sampler2D u_Texture_1;
uniform vec3 u_view_position;
uniform Light u_light;
uniform Material u_material;

void main() {
    vec4 texture_color = texture(u_material.diffuse, v_texture_coordinates);
    vec3 normal = normalize(v_normal);
    vec3 light_direction = normalize(u_light.position - v_fragment_position);

    vec3 ambient_light = u_light.ambient * texture_color.rgb;

    float diffuse = max(dot(normal, light_direction), 0.0);
    vec3 diffuse_light = u_light.diffuse * (diffuse * vec3(texture_color));

    int shininess = 32;
    // vec3 view_direction = normalize(u_view_position - v_fragment_position);
    vec3 view_direction = normalize(-v_fragment_position); // viewer position is 0,0,0 because we are in view space now
    vec3 reflected_direction = reflect(-light_direction, normal);
    float specular = pow(max(dot(view_direction, reflected_direction), 0.0), shininess);
    vec3 specular_light = u_light.specular * (specular * vec3(texture(u_material.specular, v_texture_coordinates))); // replace texture_color with specular map

    vec3 result_light = ambient_light + diffuse_light + specular_light;

    color = vec4(result_light, 1.0);
}
