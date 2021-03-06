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
#include <material.h>
#include <logging/logging.h>

#define CAPTURE_CURSOR 0
#define LOAD_SKYBOX 0


static void glfw_error_callback(int error, const char* description) {
    LOG_ERROR << "GLFW failed: " << error << ": " << description;
}


GLFWwindow* init_window(int width, int height) {
    if (!glfwInit()) {
        LOG_ERROR << "Error: GLFW Init failed!";
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
        LOG_ERROR << "GLFW CreateWindow failed!";
        glfwTerminate();
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
        LOG_ERROR << glewGetErrorString(err);
        return nullptr;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    GL_CHECK_ERRORS;
    LOG_DEBUG << "GL_DEPTH_TEST enabled";

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
    LogGlobalContext::instance()
        .attach(std::cout)
        .attach(std::cerr, Log::Level::Warning)
        .attach("gir1.log");
    LOG_INFO << "Welcome to GIR1";

    glfwSetErrorCallback(glfw_error_callback);

    int width = 1280;
    int height = 720;

    /* Create a windowed mode window and its OpenGL context */
    auto window = init_window(width, height);
    init_imgui(window);

    Renderer::init();

    LOG_INFO << "GIR1   v." << version;
    LOG_INFO << "GLEW   v." << glewGetString(GLEW_VERSION);
    LOG_INFO << "OpenGL v." << glGetString(GL_VERSION);
    LOG_INFO << "Renderer: " << glGetString(GL_RENDERER);

#if LOAD_SKYBOX
    /* Setting skybox */
    Shader skybox_shader;
    skybox_shader.load_shader(Shader::Type::Vertex, "resources/shaders/skybox.vshader")
            .load_shader(Shader::Type::Fragment, "resources/shaders/skybox.fshader")
            .compile()
            .bind();

    skybox_shader.set_uniform_1i("u_Texture_skybox", 0);

    Skybox skybox("resources/textures/skybox_space");
    skybox.shader = &skybox_shader;
    skybox.transform = glm::scale(glm::mat4(1.0f), glm::vec3(500.0f));
#endif

    /* Setting cubes */
    Shader shader;
    shader.load_shader(Shader::Type::Vertex, "resources/shaders/texture_2d.vshader")
          .load_shader(Shader::Type::Fragment, "resources/shaders/texture_2d.fshader")
          .compile()
          .bind();

    Material wooden_crate("resources/textures/container2.png", "resources/textures/container2_specular_map.png");
    wooden_crate.shader = &shader;

    shader.set_uniform_1i("u_material.diffuse", 0);
    shader.set_uniform_1i("u_material.specular", 1);

    Shader light_source_shader;
    light_source_shader.load_shader(Shader::Type::Vertex, "resources/shaders/light_source.vshader");
    light_source_shader.load_shader(Shader::Type::Fragment, "resources/shaders/light_source.fshader");
    light_source_shader.compile().bind();

    ModelAsset cube_asset = ModelAsset::load_my_model("resources/models/cube.model");
    cube_asset.material = &wooden_crate;

    std::vector<ModelInstance> models(2);
    models[0].asset = &cube_asset;
    models[0].transform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.f, 0.f, 1.f));

    models[1].asset = &cube_asset;
    models[1].transform =
        glm::scale(
            glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 3.0f)),
        glm::vec3(2.0f));


    PointLight light_source(glm::vec3(1.0f, 3.0f, 3.0f), glm::vec3(1.0f));
    Material glowing_material;
    glowing_material.shader = &light_source_shader;
    
    ModelAsset light_source_asset = ModelAsset::load_my_model("resources/models/cube.model");
    light_source_asset.material = &glowing_material;

    ModelInstance light_source_cube;
    light_source_cube.asset = &light_source_asset;
    light_source_cube.transform =
        glm::scale(glm::translate(glm::mat4(1.0f), light_source.position), glm::vec3(0.2));

    // ================ RAYMARCH INSTANCE ================ //
    Shader raymarch_shader;
    raymarch_shader.load_shader(Shader::Type::Vertex, "resources/shaders/raymarch.vs");
    raymarch_shader.load_shader(Shader::Type::Fragment, "resources/shaders/raymarch.fs");
    raymarch_shader.compile().bind();
    Material raymarch_material;
    raymarch_material.shader = &raymarch_shader;

    ModelAsset raymarch_asset = ModelAsset::load_my_model("resources/models/cube.model");
    raymarch_asset.material = &raymarch_material;

    ModelInstance raymarch_cube;
    raymarch_cube.asset = &raymarch_asset;
    raymarch_cube.transform = glm::mat4(1.0f);
    // =================================================== //

    auto& input = KeyboardState::instance();
    glfwGetCursorPos(window, &input.cursor_x, &input.cursor_y);

    Camera camera;
    camera.translate_backwards(.5f);

    double t = glfwGetTime();
    bool skybox_active = true;

    glm::mat4 projection = glm::perspective(glm::radians(30.0f), (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
    shader.set_uniform_mat4f("u_projection", projection);
#if LOAD_SKYBOX
    skybox_shader.set_uniform_mat4f("u_projection", projection);
#endif
    light_source_shader.set_uniform_mat4f("u_projection", projection);
    raymarch_shader.set_uniform_mat4f("u_projection", projection);

#if LOAD_SKYBOX
    LOG_INFO << "Press F3 to toggle skybox";
#endif
    LOG_INFO << "Esc to exit";

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
#if 1
        for (auto& model : models) {
            // Setting MVP components
            model.asset->material->shader->set_uniform_mat4f("u_model", model.transform);
            model.asset->material->shader->set_uniform_mat4f("u_view", view);

            // Setting normal matrix for normals rescaling
            // Also moved normal matrix into view space
            model.asset->material->shader->set_uniform_mat4f("u_normal_matrix", glm::transpose(glm::inverse(view * model.transform)));

            // Setting light properties
            // Light source position also is moved into view space
            model.asset->material->shader->set_uniform_vec3f("u_light.position", view * glm::vec4(light_source.position, 1.0f));
            model.asset->material->shader->set_uniform_vec3f("u_light.ambient", light_source.ambient);
            model.asset->material->shader->set_uniform_vec3f("u_light.diffuse", light_source.diffuse);
            model.asset->material->shader->set_uniform_vec3f("u_light.specular", light_source.specular);

            // Setting view position for specular light computation
            model.asset->material->shader->set_uniform_vec3f("u_view_position", camera.position);
            Renderer::draw(model);
        }
#endif

        raymarch_cube.asset->material->shader->set_uniform_vec3f("u_light.position", light_source.position);
        raymarch_cube.asset->material->shader->set_uniform_vec3f("u_camera.position", camera.position);
        raymarch_cube.asset->material->shader->set_uniform_vec3f("u_camera.direction", camera.direction);
        raymarch_cube.asset->material->shader->set_uniform_mat4f("u_model", raymarch_cube.transform);
        raymarch_cube.asset->material->shader->set_uniform_mat4f("u_view", view);
        Renderer::draw(raymarch_cube);

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
                ImGui::Text("Camera{\n  position:(%5.2f, %5.2f, %5.2f),\n  direction(%5.2f, %5.2f, %5.2f)\n}", 
                    camera.position.x, camera.position.y, camera.position.z, 
                    camera.direction.x, camera.direction.y, camera.direction.z);

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
