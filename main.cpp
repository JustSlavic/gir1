#include <stdio.h>

#include <GL/glew.h> // have to be included before glfw3
#include <GLFW/glfw3.h>


#include <version.h>
#include <defines.h>

#include <vertex_buffer.h>
#include <index_buffer.h>
#include <vertex_array.h>
#include <shader.h>


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

    Shader program;
    program.load_shader(Shader::Type::Vertex, "resources/shaders/vertex.vshader")
           .load_shader(Shader::Type::Fragment, "resources/shaders/fragment.fshader")
           .compile()
           .bind();

    Shader::Uniform uniform = program.get_uniform("u_Color");
    Shader::Uniform unif2 = program.get_uniform("u_Color");
    Shader::Uniform unif3 = program.get_uniform("u_Color");

    vertex_array.unbind();
    vertex_buffer.unbind();
    index_buffer.unbind();
    program.unbind();

    float r = 0.4;
    float dr = 0.05;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        program.bind().set_uniform_4f(uniform, r, 0.3, 0.8, 1.0);

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

    glfwTerminate();
    return 0;
}
