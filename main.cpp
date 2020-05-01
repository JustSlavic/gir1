#include <stdio.h>

#include <GL/glew.h> // have to be included before glfw3
#include <GLFW/glfw3.h>


#include <version.h>
#include <defines.h>

#include <vertex_buffer.h>
#include <index_buffer.h>
#include <vertex_array.h>


GLuint compile_shader(GLuint type, const char* source) {
    GLuint id = glCreateShader(type);
    GL_CALL(glShaderSource(id, 1, &source, nullptr));
    GL_CALL(glCompileShader(id));

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // char* message = (char*) alloca(length * sizeof(char)); // C function for dynamically allocate on stack
        char* message = new char[length];

        GL_CALL(glGetShaderInfoLog(id, length, &length, message));
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


int main(int argc, char** argv, char** env) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Syncronise swap interval with vsync (60fps?) */
    glfwSwapInterval(1);

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

    VertexArray vertex_array;
    VertexBuffer vertex_buffer(positions, 4 * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(2);

    vertex_array.add_buffer(vertex_buffer, layout);

    IndexBuffer index_buffer(indices, 3*2);

    std::string vertex_shader = read_whole_file("resources/shaders/vertex.vshader");
    std::string fragment_shader = read_whole_file("resources/shaders/fragment.fshader");

    GLuint shader = crate_shader(vertex_shader.data(), fragment_shader.data());
    GL_CALL(glUseProgram(shader));

    GLint location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);

    GL_CALL(glBindVertexArray(0));
    GL_CALL(glUseProgram(0));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0))
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))


    float r = 0.4;
    float dr = 0.05;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        GL_CALL(glUseProgram(shader));
        GL_CALL(glUniform4f(location, r, 0.3, 0.8, 1.0));

        vertex_array.bind();
        index_buffer.bind();

        GL_CALL(glDrawElements(GL_TRIANGLES, index_buffer.count, GL_UNSIGNED_INT, nullptr));

        if (r < 0.0) dr = 0.05;
        if (r > 1.0) dr = -0.05;

        r += dr;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GL_CALL(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}
