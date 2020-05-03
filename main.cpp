// std imports
#include <cstdio>

// opengl imports
#include <GL/glew.h> // have to be included before glfw3
#include <GLFW/glfw3.h>

// opengl math imports
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// imgui imports
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// my imports
#include <vertex_array.h>
#include <vertex_buffer.h>
#include <index_buffer.h>
#include <shader.h>
#include <renderer.h>
#include <texture.h>
#include <input.h>
#include <version.h>


static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


int main(int argc, char** argv, char** env) {
    glfwSetErrorCallback(glfw_error_callback);

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
    GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /* Set input callback */
    glfwSetKeyCallback(window, key_callback);
    /* Set mouse position callback */
    glfwSetCursorPosCallback(window, cursor_position_callback);

    /* Syncronise swap interval with vsync (60fps?) */
    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      return -1;
    }

    // ========== IMGUI =========

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // load font
    io.Fonts->AddFontFromFileTTF("resources/fonts/Cousine-Regular.ttf", 15.0f);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // ==========================


    fprintf(stdout, "Status: Using GIR1 v.%s\n", version);
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "Status: Using OpenGL v.%s\n", glGetString(GL_VERSION));

    // 4x3 projection
    glm::mat4 projection_matrix = glm::ortho(-width/2.0, width/2.0, -height/2.0, height/2.0, -1.0, 1.0);
    glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(200, 200, 0));

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

//    Shader::Uniform uniform = shader.get_uniform("u_Color");
    shader.set_uniform_1i("u_Texture", 0);

    float camera_x = 0.0f;
    float camera_y = 0.0f;
    float r = 0.4;
    float dr = 0.05;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        Renderer::clear();

//        shader.bind().set_uniform_4f(uniform, r, 0.3, 0.8, 1.0);

        glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-camera_x, -camera_y, 0));
        glm::mat4 mvp = projection_matrix * view_matrix * model_matrix;

        shader.set_uniform_mat4f("u_MVP", mvp);

        Renderer::draw(vertex_array, index_buffer, shader);

        if (r < 0.0) dr = 0.05;
        if (r > 1.0) dr = -0.05;

        r += dr;


        {
            /*  Imgui rendering */

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            {
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("camera_x", &camera_x, -0.5f*width, 0.5f*width);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("camera_y", &camera_y, -0.5f*height, 0.5f*height);            // Edit 1 float using a slider from 0.0f to 1.0f

                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
