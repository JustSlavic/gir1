#include <shader.h>
#include <utils.h>
#include <defines.h>
#include <logging/logging.h>

#include <GL/glew.h>
#include <vector>


LOG_CONTEXT("shader");

Shader::~Shader() {
    glDeleteProgram(id); GL_CHECK_ERRORS;
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

const char *shader_type_to_c_str(Shader::Type type) {
    switch (type) {
        case Shader::Type::Vertex: return "Vertex";
        case Shader::Type::Fragment: return "Fragment";
    }

    return "";
}


GLuint compile_shader(GLuint type, const char* source) {
    GLuint id = glCreateShader(type); GL_CHECK_ERRORS;
    glShaderSource(id, 1, &source, nullptr); GL_CHECK_ERRORS;
    glCompileShader(id); GL_CHECK_ERRORS;

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); GL_CHECK_ERRORS;
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); GL_CHECK_ERRORS;

        char* message = new char[length];

        glGetShaderInfoLog(id, length, &length, message); GL_CHECK_ERRORS;
        LOG_ERROR << "Error in compiling shader!";
        LOG_ERROR << message;

        glDeleteShader(id); GL_CHECK_ERRORS;
        delete[] message;
        return 0;
    }

    return id;
}


Shader &Shader::compile() {
    id = glCreateProgram(); GL_CHECK_ERRORS;

    std::vector<GLuint> shader_ids;

    for (auto& pair : sources) {
        GLuint shader_id = compile_shader(shader_type_to_gl_enum(pair.first), pair.second.c_str());
        glAttachShader(id, shader_id); GL_CHECK_ERRORS;
        shader_ids.push_back(shader_id);

        LOG_DEBUG << shader_type_to_c_str(pair.first) << " shader compiled successfully";
    }

    glLinkProgram(id); GL_CHECK_ERRORS;
    glValidateProgram(id); GL_CHECK_ERRORS;

    LOG_DEBUG << "Shaders linked successfully";

    for (auto& shader_id : shader_ids) {
        glDeleteShader(shader_id); GL_CHECK_ERRORS;
    }

    return *this;
}


Shader& Shader::bind() {
    glUseProgram(id); GL_CHECK_ERRORS;
    return *this;
}


void Shader::unbind() {
    glUseProgram(0); GL_CHECK_ERRORS;
}

Shader::Uniform Shader::get_uniform(const char *name) {
    auto found = uniform_cache.find(name);
    if (found != uniform_cache.end()) return {found->second};

    GLint location = glGetUniformLocation(id, name); GL_CHECK_ERRORS;
//    ASSERT(location != -1);

    uniform_cache.emplace(std::string(name), Uniform(location));
    return {location};
}

Shader &Shader::set_uniform_4f(Shader::Uniform uniform, float x1, float x2, float x3, float x4) {
    this->bind();
    glUniform4f(uniform.location, x1, x2, x3, x4); GL_CHECK_ERRORS;
    return *this;
}

Shader &Shader::set_uniform_4f(const char *name, float x1, float x2, float x3, float x4) {
    Uniform uniform = get_uniform(name);
    set_uniform_4f(uniform, x1, x2, x3, x4);
    return *this;
}

Shader &Shader::set_uniform_mat4f(Shader::Uniform uniform, const glm::mat4 &matrix) {
    this->bind();
    glUniformMatrix4fv(uniform.location, 1, GL_FALSE, &matrix[0][0]); GL_CHECK_ERRORS;
    return *this;
}

Shader &Shader::set_uniform_mat4f(const char *name, const glm::mat4 &matrix) {
    Uniform uniform = get_uniform(name);
    set_uniform_mat4f(uniform, matrix);
    return *this;
}

Shader &Shader::set_uniform_1i(Shader::Uniform uniform, int x) {
    this->bind();
    glUniform1i(uniform.location, x); GL_CHECK_ERRORS;
    return *this;
}

Shader &Shader::set_uniform_1i(const char *name, int x) {
    Uniform uniform = get_uniform(name);
    set_uniform_1i(uniform, x);
    return *this;
}

Shader &Shader::set_uniform_3f(Shader::Uniform uniform, float x1, float x2, float x3) {
    this->bind();
    glUniform3f(uniform.location, x1, x2, x3); GL_CHECK_ERRORS;
    return *this;
}

Shader &Shader::set_uniform_3f(const char *name, float x1, float x2, float x3) {
    Uniform uniform = get_uniform(name);
    set_uniform_3f(uniform, x1, x2, x3);
    return *this;
}

Shader &Shader::set_uniform_vec3f(Shader::Uniform uniform, const glm::vec3 &vector) {
    this->bind();
    glUniform3fv(uniform.location, 1, &vector.x);
    return *this;
}

Shader &Shader::set_uniform_vec3f(const char *name, const glm::vec3 &vector) {
    Uniform uniform = get_uniform(name);
    set_uniform_vec3f(uniform, vector);
    return *this;
}

Shader::Uniform::Uniform(int location) :location(location) {}
