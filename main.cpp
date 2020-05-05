// std imports
#include <cstdio>
#include <iostream>
#include <ostream>
#include <cmath>

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
#include <camera.h>
#include <version.h>


static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


std::ostream& operator<<(std::ostream& os, glm::mat4 m) {
    for (int i = 0; i < 4; ++i) {
        os << "|" << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << "|\n";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, glm::vec4 v) {
    return os << "[" << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << "]\n";
}


GLFWwindow* init_window(int width, int height) {
    if (!glfwInit()) {
        fprintf(stderr, "Error: GLFW Init failed!\n");
        std::exit(ERROR_GLFW_FAILED);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4); // Enable 4x anti aliasing

    GLFWwindow* window = glfwCreateWindow(width, height, "Gir1", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Error: GLFW CreateWindow failed!\n");
        std::exit(ERROR_GLFW_FAILED);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /* Set input callback */
    glfwSetKeyCallback(window, key_callback);
    /* Set mouse position callback */
    glfwSetCursorPosCallback(window, cursor_position_callback);
    /* Set mouse buttons presses */
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    /* Enable vsync */
    glfwSwapInterval(1);
    /* Disable cursor visibility */
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return nullptr;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    return window;
}


void init_imgui(GLFWwindow *window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    io.Fonts->AddFontFromFileTTF("resources/fonts/Cousine-Regular.ttf", 15.0f);
}


int main(int argc, char** argv, char** env) {
    glfwSetErrorCallback(glfw_error_callback);

    int width = 1280;
    int height = 720;

    /* Create a windowed mode window and its OpenGL context */
    auto window = init_window(width, height);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    init_imgui(window);

    Renderer::init();
    fprintf(stdout, "GIR1   v.%s\n", version);
    fprintf(stdout, "GLEW   v.%s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "OpenGL v.%s\n", glGetString(GL_VERSION));
    fprintf(stdout, "Renderer: %s\n\n", glGetString(GL_RENDERER));

    VertexArray vertex_array;

    VertexBuffer vertex_buffer(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);

    vertex_array.add_buffer(vertex_buffer, layout);

//    IndexBuffer index_buffer(indices, 3*2*6);

    Texture texture("resources/textures/wall.png");

    Shader shader;
    shader.load_shader(Shader::Type::Vertex, "resources/shaders/vertex.vshader")
          .load_shader(Shader::Type::Fragment, "resources/shaders/fragment.fshader")
          .compile()
          .bind();

    shader.set_uniform_1i("u_Texture", 0);

    auto& input = KeyboardState::instance();
    glfwGetCursorPos(window, &input.cursor_x, &input.cursor_y);

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
    glm::mat4 projection = glm::perspective(glm::radians(30.0f), (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Camera camera;

    double t = glfwGetTime();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        Renderer::clear();

        auto dt = static_cast<float>(glfwGetTime() - t);
        t = glfwGetTime();

        if (input.W_pressed) { camera.translate_forward(dt); }
        if (input.S_pressed) { camera.translate_backwards(dt); }
        if (input.A_pressed) { camera.translate_left(dt); }
        if (input.D_pressed) { camera.translate_right(dt); }
        if (input.R_pressed) { camera.translate_up(dt); }
        if (input.F_pressed) { camera.translate_down(dt); }

        if (input.UP_pressed) { camera.rotate_up(dt); }
        if (input.DOWN_pressed) { camera.rotate_down(dt); }
        if (input.LEFT_pressed) { camera.rotate_left(dt); }
        if (input.RIGHT_pressed) { camera.rotate_right(dt); }

        camera.rotate_right((float)input.cursor_dx * 0.5f * dt);
        camera.rotate_up((float)input.cursor_dy * 0.5f * dt);

        glm::mat4 view = camera.get_view_matrix();

        glm::mat4 mvp = projection * view * model;

        shader.set_uniform_mat4f("u_MVP", mvp);

        Renderer::draw(vertex_array, shader);

        {
            /*  DEAR IM GUI */

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                static int counter = 0;

                ImGui::Begin("Girl debug view"); // Create a window called "Hello, world!" and append into it.
                ImGui::Text("Camera positions:"); // Display some text (you can use a format strings too)
//                ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
//                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("player_x", &camera.position.x, -100, 100);
                ImGui::SliderFloat("player_y", &camera.position.y, -100, 100);
                ImGui::SliderFloat("player_z", &camera.position.z, -100, 100);

                ImGui::Text("Forward vector: (%5.2lf, %5.2lf, %5.2lf)", camera.direction.x, camera.direction.y, camera.direction.z);
                ImGui::Text("Pitch: %f", camera.pitch);
                ImGui::Text("Yaw: %f", camera.yaw);

                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                // Buttons return true when clicked (most widgets return true when edited/activated)
                if (ImGui::Button("Button")) { counter++; }
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

//                double cursor_x, cursor_y;
//                glfwGetCursorPos(window, &cursor_x, &cursor_y);

                ImGui::Text("Cursor: (%5.2lf, %5.2lf)", input.cursor_x, input.cursor_y);
                ImGui::Text("Cursor dr: (%5.2lf, %5.2lf)", input.cursor_dx, input.cursor_dy);
                ImGui::Text("LMB drag: (%5.2lf, %5.2lf)", input.LMB_drag_x, input.LMB_drag_y);
                ImGui::Text("RMB drag: (%5.2lf, %5.2lf)", input.RMB_drag_x, input.RMB_drag_y);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS); dt = %.3f", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate, dt);
                ImGui::End();
            }

            input.cursor_dx = 0;
            input.cursor_dy = 0;

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
