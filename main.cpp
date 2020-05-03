#include <cstdio>

#include <GL/glew.h> // have to be included before glfw3
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <version.h>

#include <vertex_buffer.h>
#include <index_buffer.h>
#include <vertex_array.h>
#include <shader.h>
#include <renderer.h>
#include <texture.h>


int main(int argc, char** argv, char** env) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    int width = 1280;
    int height = 720;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
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

    // 4x3 projection
    glm::mat4 projection_matrix = glm::ortho(-width/2.0, width/2.0, -height/2.0, height/2.0, -1.0, 1.0);
    glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-200, -200, 0));
    glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(200, 200, 0));

    glm::mat4 mvp = projection_matrix * view_matrix * model_matrix;

    float positions[4*4] = {
    // external coords   texture coords
        -1.0, -1.0,  0.0,  0.0,  // 0 - bottom left
         1.0, -1.0,  1.0,  0.0,  // 1 - bottom right
         1.0,  1.0,  1.0,  1.0,  // 2 - top right
        -1.0,  1.0,  0.0,  1.0,  // 3 - top left
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    Renderer::init();

    VertexArray vertex_array;
    VertexBuffer vertex_buffer(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);

    vertex_array.add_buffer(vertex_buffer, layout);

    IndexBuffer index_buffer(indices, 3*2);

    Texture wall_texture("resources/textures/fire.png");

    Shader shader;
    shader.load_shader(Shader::Type::Vertex, "resources/shaders/vertex.vshader")
           .load_shader(Shader::Type::Fragment, "resources/shaders/fragment.fshader")
           .compile()
           .bind();

    Shader::Uniform uniform = shader.get_uniform("u_Color");
    shader.set_uniform_mat4f("u_MVP", mvp);
    shader.set_uniform_1i("u_Texture", 0);

    float r = 0.4;
    float dr = 0.05;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        Renderer::clear();

        shader.bind().set_uniform_4f(uniform, r, 0.3, 0.8, 1.0);

        Renderer::draw(vertex_array, index_buffer, shader);

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
