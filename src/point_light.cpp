#include <point_light.h>
#include <utility>


PointLight::PointLight(glm::vec3 position, glm::vec3 color)
    : position(position)
    , ambient(ambient_strength)
    , diffuse(color)
    , specular(specular_strength)
{
}
