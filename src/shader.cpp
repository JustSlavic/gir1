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

        if (pair.first == Type::Vertex) {
            printf("Vertex ");
        } else if (pair.first == Type::Fragment) {
            printf("Fragment ");
        }
        printf("shader is compiled successfully\n");
    }

    glLinkProgram(id); GL_CHECK_ERRORS;
    glValidateProgram(id); GL_CHECK_ERRORS;

    printf("Shaders linked successfully\n");

    for (auto& shader_id : shader_ids) {
        glDeleteShader(shader_id); GL_CHECK_ERRORS;
    }

    return *this;
}


Shader& Shader::bind() {
    glUseProgram(id); GL_CHECK_ERRORS;
    return *this;
}


Shader& Shader::unbind() {
    glUseProgram(0); GL_CHECK_ERRORS;
    return *this;
}

Shader::Uniform Shader::get_uniform(const char *name) {
    auto found = uniform_cache.find(name);
    if (found != uniform_cache.end()) return {found->second};

    GLint location = glGetUniformLocation(id, name); GL_CHECK_ERRORS;
    ASSERT(location != -1);

    uniform_cache.emplace(std::string(name), Uniform(location));
    return {location};
}

Shader &Shader::set_uniform_4f(Shader::Uniform uniform, float x1, float x2, float x3, float x4) {
    glUniform4f(uniform.location, x1, x2, x3, x4); GL_CHECK_ERRORS;
    return *this;
}

Shader &Shader::set_uniform_4f(const char *name, float x1, float x2, float x3, float x4) {
    Uniform uniform = get_uniform(name);
    set_uniform_4f(uniform, x1, x2, x3, x4);
    return *this;
}

Shader &Shader::set_uniform_mat4f(Shader::Uniform uniform, const glm::mat4 &matrix) {
    GL_CALL(glUniformMatrix4fv(uniform.location, 1, GL_FALSE, &matrix[0][0]));
    return *this;
}

Shader &Shader::set_uniform_mat4f(const char *name, const glm::mat4 &matrix) {
    Uniform uniform = get_uniform(name);
    set_uniform_mat4f(uniform, matrix);
    return *this;
}

Shader &Shader::set_uniform_1i(Shader::Uniform uniform, int x) {
    GL_CALL(glUniform1i(uniform.location, x));
    return *this;
}

Shader &Shader::set_uniform_1i(const char *name, int x) {
    Uniform uniform = get_uniform(name);
    set_uniform_1i(uniform, x);
    return *this;
}

Shader::Uniform::Uniform(int location) :location(location) {}
