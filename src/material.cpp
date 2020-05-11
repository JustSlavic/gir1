#include <material.h>

Material::Material(const char *texture, const char *specular_map)
    : shader(nullptr)
    , diffuse_map(new Texture(texture))
    , specular_map(new Texture(specular_map))
{
}

void Material::bind() const {
    if (shader) shader->bind();
    if (diffuse_map) diffuse_map->bind(0);
    if (specular_map) specular_map->bind(1);
}

void Material::unbind() {
    Shader::unbind();
    Texture::unbind();
}
