#ifndef GIR1_SKYBOX_H
#define GIR1_SKYBOX_H

#include <shader.h>


struct Skybox {
    unsigned int vertex_array_id = 0;
    unsigned int vertex_buffer_id = 0;
    unsigned int texture_id = 0;
    glm::mat4 transform;

    Shader *shader{nullptr};

    explicit Skybox(const char* dir_path);
    ~Skybox();
    Skybox(const Skybox&) = delete;

    void bind(unsigned int slot = 0) const;
    static void unbind();
};

#endif //GIR1_SKYBOX_H
