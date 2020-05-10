#include <point_light.h>
#include <utility>


PointLight::PointLight(glm::vec3 position, glm::vec3 color)
    : position(position)
    , ambient(0.1f)
    , diffuse(color)
    , specular(0.1f)
{
}
