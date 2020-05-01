#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifdef __linux__
#include <unistd.h>
#endif


#include <GL/glew.h> // have to be included before glfw3
#include <GLFW/glfw3.h>

#include <version.h>


std::string get_current_path() {
#ifdef __linux__
    // ssize_t readlink(const char *path, char *buf, size_t bufsiz);
    char* dir_name = get_current_dir_name();
    std::string result(dir_name);
    free(dir_name);
#endif

    return result;
}


std::string read_whole_file(const char *filename) {
    std::ifstream input(filename, std::ios::in | std::ios::binary);
    std::ostringstream content;

    if (input.good()) {
        content << input.rdbuf();
    }

    return content.str();
}


GLuint compile_shader(GLuint type, const char* source) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // char* message = (char*) alloca(length * sizeof(char)); // C function for dynamically allocate on stack
        char* message = new char[length];

        glGetShaderInfoLog(id, length, &length, message);
        printf("Error in compiling shader!\n");
        printf("Error: %s\n", message);

        glDeleteShader(id);
        delete[] message;
        return 0;
    }

    return id;
}


GLuint crate_shader(const char* vertex_shader, const char* fragment_shader) {
    GLuint program = glCreateProgram();
    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


void check_gl_error() {
    while (true) {
        switch (glGetError()) {
        case GL_INVALID_ENUM:
            printf("Error: An unacceptable value is specified for an enumerated argument.\n");
            printf("       The offending command is ignored and has no other side effect than to set the error flag.\n");
            std::exit(GL_INVALID_ENUM);
        case GL_INVALID_VALUE:
            printf("Error: A numeric argument is out of range.\n");
            printf("       The offending command is ignored and has no other side effect than to set the error flag.\n");
            std::exit(GL_INVALID_ENUM);
        case GL_INVALID_OPERATION:
            printf("Error: The specified operation is not allowed in the current state.\n");
            printf("       The offending command is ignored and has no other side effect than to set the error flag.\n");
            std::exit(GL_INVALID_ENUM);
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            printf("Error: The framebuffer object is not complete.\n");
            printf("       The offending command is ignored and has no other side effect than to set the error flag.\n");
            std::exit(GL_INVALID_ENUM);
        case GL_OUT_OF_MEMORY:
            printf("Error: There is not enough memory left to execute the command.\n");
            printf("       The state of the GL is undefined, except for the state of the error flags,");
            printf("       after this error is recorded.\n");
            std::exit(GL_INVALID_ENUM);
        case GL_STACK_UNDERFLOW:
            printf("Error: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n");
            std::exit(GL_INVALID_ENUM);
        case GL_STACK_OVERFLOW:
            printf("Error: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n");
            std::exit(GL_INVALID_ENUM);
        case GL_NO_ERROR:  // No error has been recorded. The value of this symbolic constant is guaranteed to be 0.
            return;
        }
    }
}



int main(int argc, char** argv, char** env) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      return -1;
    }

    fprintf(stdout, "Status: Using GIR1 v.%s\n", version);
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "Status: Using OpenGL v.%s\n", glGetString(GL_VERSION));

    float positions[4*2] = {
        -0.5, -0.5,  // 0 - bottom left
         0.5, -0.5,  // 1 - bottom right
         0.5,  0.5,  // 2 - top right
        -0.5,  0.5,  // 3 - top left
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    GLuint buffer; // for storing id of the vertex buffer
    glGenBuffers(1, &buffer); // generate me 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // select this buffer (I'm about to work with it)
    glBufferData(GL_ARRAY_BUFFER, 6*2*sizeof(float), positions, GL_STATIC_DRAW);

    GLuint index_buffer; // for storing id of the index buffer
    glGenBuffers(1, &index_buffer); // generate me 1 buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); // select this buffer (I'm about to work with it)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*2*sizeof(GLuint), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        /*index*/ 0,
        /*size of vertex*/ 2,
        /*type*/ GL_FLOAT,
        /*normalize?*/ GL_FALSE,
        /*stride*/ sizeof(float) * 2,
        /*pointer*/ NULL);


    std::string vertex_shader = read_whole_file("resources/shaders/vertex.shader");
    std::string fragment_shader = read_whole_file("resources/shaders/fragment.shader");

    GLuint shader = crate_shader(vertex_shader.data(), fragment_shader.data());
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        check_gl_error();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
