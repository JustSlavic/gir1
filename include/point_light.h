#ifndef GIR1_POINT_LIGHT_H
#define GIR1_POINT_LIGHT_H


#include <glm/glm.hpp>

struct PointLight {
    glm::vec3 position;

    float ambient_strength = 0.2f;
    float specular_strength = 1.0f;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    PointLight(glm::vec3 position, glm::vec3 color);
};

#endif //GIR1_POINT_LIGHT_H
