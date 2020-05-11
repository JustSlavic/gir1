#ifndef GIR1_MATERIAL_H
#define GIR1_MATERIAL_H


#include <shader.h>
#include <texture.h>

struct Material {
    Shader *shader{nullptr};
    Texture *diffuse_map{nullptr};
    Texture *specular_map{nullptr};

    Material() = default;
    Material(const char *texture, const char *specular_map);

    void bind() const;
    static void unbind();
};

#endif //GIR1_MATERIAL_H
