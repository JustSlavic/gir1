#include <shader.h>
#include <utils.h>
#include <defines.h>

#include <GL/glew.h>
#include <vector>


Shader::~Shader() {
    GL_CALL(glDeleteProgram(id));
}


Shader &Shader::load_shader(Shader::Type type, const char *filename) {
    std::string source = read_whole_file(filename);
    sources.emplace(type, std::move(source));

    return *this;
}


GLuint shader_type_to_gl_enum(Shader::Type type) {
    switch (type) {
        case Shader::Type::Vertex: return GL_VERTEX_SHADER;
        case Shader::Type::Fragment: return GL_FRAGMENT_SHADER;
    }

    return 0;
}


GLuint compile_shader(GLuint type, const char* source) {
    GL_CALL(GLuint id = glCreateShader(type));
    GL_CALL(glShaderSource(id, 1, &source, nullptr));
    GL_CALL(glCompileShader(id));

    int result;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char* message = new char[length];

        GL_CALL(glGetShaderInfoLog(id, length, &length, message));
        printf("Error in compiling shader!\n");
        printf("Error: %s\n", message);

        GL_CALL(glDeleteShader(id));
        delete[] message;
        return 0;
    }

    return id;
}


Shader &Shader::compile() {
    GL_CALL(id = glCreateProgram());

    std::vector<GLuint> shader_ids;

    for (auto& pair : sources) {
        GLuint shader_id = compile_shader(shader_type_to_gl_enum(pair.first), pair.second.c_str());
        GL_CALL(glAttachShader(id, shader_id));
        shader_ids.push_back(shader_id);
    }

    GL_CALL(glLinkProgram(id));
    GL_CALL(glValidateProgram(id));

    for (auto& shader_id : shader_ids) {
        GL_CALL(glDeleteShader(shader_id));
    }

    return *this;
}


Shader& Shader::bind() {
    GL_CALL(glUseProgram(id));
    return *this;
}


Shader& Shader::unbind() {
    GL_CALL(glUseProgram(0));
    return *this;
}

Shader::Uniform Shader::get_uniform(const char *name) {
    auto found = uniform_cache.find(name);
    if (found != uniform_cache.end()) return {found->second};

    GLint location = glGetUniformLocation(id, name);
    ASSERT(location != -1);

    uniform_cache.emplace(std::string(name), Uniform(location));
    return {location};
}

Shader &Shader::set_uniform_4f(Shader::Uniform uniform, float x1, float x2, float x3, float x4) {
    GL_CALL(glUniform4f(uniform.location, x1, x2, x3, x4));
    return *this;
}

Shader &Shader::set_uniform_4f(const char *name, float x1, float x2, float x3, float x4) {
    Uniform uniform = get_uniform(name);
    set_uniform_4f(uniform, x1, x2, x3, x4);
    return *this;
}

Shader::Uniform::Uniform(int location) :location(location) {}
