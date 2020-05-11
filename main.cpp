// std imports
#include <cstdio>
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
#include <model_asset.h>
#include <model_instance.h>
#include <vertex_array.h>
#include <vertex_buffer.h>
#include <index_buffer.h>
#include <shader.h>
#include <renderer.h>
#include <texture.h>
#include <skybox.h>
#include <input.h>
#include <camera.h>
#include <version.h>
#include <point_light.h>

#define CAPTURE_CURSOR 0
#define LOAD_SKYBOX 0


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

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
#if CAPTURE_CURSOR
    /* Disable cursor visibility */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

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

    init_imgui(window);

    Renderer::init();
    fprintf(stdout, "GIR1   v.%s\n", version);
    fprintf(stdout, "GLEW   v.%s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "OpenGL v.%s\n", glGetString(GL_VERSION));
    fprintf(stdout, "Renderer: %s\n\n", glGetString(GL_RENDERER));

#if LOAD_SKYBOX
    /* Setting skybox */
    Shader skybox_shader;
    skybox_shader.load_shader(Shader::Type::Vertex, "resources/shaders/skybox.vshader")
            .load_shader(Shader::Type::Fragment, "resources/shaders/skybox.fshader")
            .compile()
            .bind();

    skybox_shader.set_uniform_1i("u_Texture_skybox", 0);

    Skybox skybox("resources/textures/skybox_8");
    skybox.shader = &skybox_shader;
    skybox.transform = glm::scale(glm::mat4(1.0f), glm::vec3(500.0f));
#endif

    /* Setting cubes */
    Texture texture("resources/textures/container2.png");
    Texture specular_map("resources/textures/container2_specular_map.png");

    Shader shader;
    shader.load_shader(Shader::Type::Vertex, "resources/shaders/texture_2d.vshader")
          .load_shader(Shader::Type::Fragment, "resources/shaders/texture_2d.fshader")
          .compile()
          .bind();

    shader.set_uniform_1i("u_material.diffuse", 0);
    shader.set_uniform_1i("u_material.specular", 1);

    Shader light_source_shader;
    light_source_shader.load_shader(Shader::Type::Vertex, "resources/shaders/light_source.vshader");
    light_source_shader.load_shader(Shader::Type::Fragment, "resources/shaders/light_source.fshader");
    light_source_shader.compile().bind();

    ModelAsset cube_asset = ModelAsset::load_my_model("resources/models/cube.model");
    cube_asset.texture = &texture;
    cube_asset.specular_map = &specular_map;
    cube_asset.shader = &shader;

    std::vector<ModelInstance> models(2);
    models[0].asset = &cube_asset;
    models[0].transform = glm::mat4(1.0f);

    models[1].asset = &cube_asset;
    models[1].transform =
        glm::scale(
        glm::translate(
        glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 3.0f)),
        glm::vec3(2.0f));


    PointLight light_source(glm::vec3(1.0f, 3.0f, 3.0f), glm::vec3(1.0f));

    ModelAsset light_source_asset = ModelAsset::load_my_model("resources/models/cube.model");
    light_source_asset.shader = &light_source_shader;

    ModelInstance light_source_cube;
    light_source_cube.asset = &light_source_asset;
    light_source_cube.transform =
        glm::scale(glm::translate(glm::mat4(1.0f), light_source.position), glm::vec3(0.2));

    auto& input = KeyboardState::instance();
    glfwGetCursorPos(window, &input.cursor_x, &input.cursor_y);

    Camera camera;
    double t = glfwGetTime();
    bool skybox_active = false;

    glm::mat4 projection = glm::perspective(glm::radians(30.0f), (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
    shader.set_uniform_mat4f("u_projection", projection);
#if LOAD_SKYBOX
    skybox_shader.set_uniform_mat4f("u_projection", projection);
#endif
    light_source_shader.set_uniform_mat4f("u_projection", projection);

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

        if (input.F3_pressed) { skybox_active = !skybox_active; input.F3_pressed = false; }

#if CAPTURE_CURSOR
        // Mouse input
        camera.rotate_right((float) input.cursor_dx * 0.5f * dt);
        camera.rotate_up((float) input.cursor_dy * 0.5f * dt);
#endif

        glm::mat4 view = camera.get_view_matrix();

#if LOAD_SKYBOX
        if (skybox_active) {
            skybox_shader.set_uniform_mat4f("u_model", skybox.transform);
            skybox_shader.set_uniform_mat4f("u_view", view);
            Renderer::draw(skybox);
        }
#endif

        {
            light_source_shader.set_uniform_mat4f("u_model", light_source_cube.transform);
            light_source_shader.set_uniform_mat4f("u_view", view);
            light_source_shader.set_uniform_vec3f("u_light_color", light_source.diffuse);
            Renderer::draw(light_source_cube);
        }

        for (auto& model : models) {
            // Setting MVP components
            model.asset->shader->set_uniform_mat4f("u_model", model.transform);
            model.asset->shader->set_uniform_mat4f("u_view", view);

            // Setting normal matrix for normals rescaling
            // Also moved normal matrix into view space
            model.asset->shader->set_uniform_mat4f("u_normal_matrix", glm::transpose(glm::inverse(view * model.transform)));

            // Setting light properties
            // Light source position also is moved into view space
            model.asset->shader->set_uniform_vec3f("u_light.position", view * glm::vec4(light_source.position, 1.0f));
            model.asset->shader->set_uniform_vec3f("u_light.ambient", light_source.ambient);
            model.asset->shader->set_uniform_vec3f("u_light.diffuse", light_source.diffuse);
            model.asset->shader->set_uniform_vec3f("u_light.specular", light_source.specular);

            // Setting view position for specular light computation
            model.asset->shader->set_uniform_vec3f("u_view_position", camera.position);
            Renderer::draw(model);
        }

        {
            /*  DEAR IM GUI */

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                static int counter = 0;

                ImGui::Begin("Girl debug view"); // Create a window called "Hello, world!" and append into it.
                ImGui::SliderFloat("Light x:", &light_source.position.x, -5, 5);
                ImGui::SliderFloat("Light y:", &light_source.position.y, -5, 5);
                ImGui::SliderFloat("Light z:", &light_source.position.z, -5, 5);

                ImGui::SliderFloat("Light ambient:", &light_source.ambient_strength, 0, 1);
                ImGui::SliderFloat("Light specular:", &light_source.specular_strength, 0, 10);

                // Follow light source with cube that represents it
                light_source_cube.transform[3] = glm::vec4(light_source.position, 1.0f);

//                ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
//                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::ColorEdit3("Light color", &light_source.diffuse.x); // Edit 3 floats representing a color

                light_source.ambient = light_source.diffuse * light_source.ambient_strength;
                light_source.specular = light_source.diffuse * light_source.specular_strength;

                // Buttons return true when clicked (most widgets return true when edited/activated)
                if (ImGui::Button("Button")) { counter++; }
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Cursor: (%5.2lf, %5.2lf)", input.cursor_x, input.cursor_y);
                // ImGui::Text("Cursor dr: (%5.2lf, %5.2lf)", input.cursor_dx, input.cursor_dy);
                // ImGui::Text("LMB drag: (%5.2lf, %5.2lf)", input.LMB_drag_x, input.LMB_drag_y);
                // ImGui::Text("RMB drag: (%5.2lf, %5.2lf)", input.RMB_drag_x, input.RMB_drag_y);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS); dt = %.3f", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate, dt);
                ImGui::End();
            }

            input.cursor_dx = 0;
            input.cursor_dy = 0;
            input.LMB_drag_x = 0;
            input.LMB_drag_y = 0;
            input.RMB_drag_x = 0;
            input.RMB_drag_y = 0;

            // Render Dear ImGui
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
