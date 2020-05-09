#ifndef GIR1_SHADER_H
#define GIR1_SHADER_H

#include <unordered_map>
#include <glm/glm.hpp>


struct Shader {
    enum class Type {
        Vertex,
        Fragment
    };

    struct Uniform {
        int location;

        Uniform(int);
    };

    Shader() = default;
    Shader(const Shader&) = delete;
    Shader(Shader&&) = default;
    ~Shader();

    unsigned int id = 0;
    std::unordered_map<Type, std::string> sources;
    std::unordered_map<std::string, Uniform> uniform_cache;

    Shader& load_shader(Type type, const char *filename);
    Shader& compile();

    Uniform get_uniform(const char *name);

    Shader& set_uniform_1i(Uniform uniform, int x);
    Shader& set_uniform_1i(const char *name, int x);

    Shader& set_uniform_3f(Uniform uniform, float x1, float x2, float x3);
    Shader& set_uniform_3f(const char *name, float x1, float x2, float x3);

    Shader& set_uniform_vec3f(Uniform uniform, const glm::vec3 &vector);
    Shader& set_uniform_vec3f(const char *name, const glm::vec3 &vector);

    Shader& set_uniform_4f(Uniform uniform, float x1, float x2, float x3, float x4);
    Shader& set_uniform_4f(const char *name, float x1, float x2, float x3, float x4);

    Shader& set_uniform_mat4f(Uniform uniform, const glm::mat4& matrix);
    Shader& set_uniform_mat4f(const char *name, const glm::mat4& matrix);

    Shader& bind();
    static void unbind();
};


#endif // GIR1_SHADER_H
