#ifndef GIR1_SHADER
#define GIR1_SHADER

#include <unordered_map>


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
    Shader& set_uniform_4f(Uniform uniform, float x1, float x2, float x3, float x4);
    Shader& set_uniform_4f(const char *name, float x1, float x2, float x3, float x4);

    Shader& bind();
    Shader& unbind();
};


#endif // GIR1_SHADER
