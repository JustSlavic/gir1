#include <GL/glew.h> // have to be included before glfw3

// imgui imports
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// gir1 imports
#include <application.h>
#include <renderer.h>
#include <input.h>
#include <defines.h>
#include <version.h>
#include <logging/logging.h>


LOG_CONTEXT("application");


static void glfw_error_callback(int error, const char* description) {
    LOG_ERROR << "GLFW failed: " << error << ": " << description;
}

Application::Application(int width, int height)
    : window(nullptr)
    , width(width)
    , height(height)
{
    init_window(width, height);
    init_imgui();
    Renderer::init();

    LOG_INFO << "GIR1   v." << version;
    LOG_INFO << "GLEW   v." << glewGetString(GLEW_VERSION);
    LOG_INFO << "OpenGL v." << glGetString(GL_VERSION);
    LOG_INFO << "Renderer: " << glGetString(GL_RENDERER);
}

void Application::load_scene() {

}

void Application::run() {

}

void Application::init_window(int width, int height) {
    LOG_CONTEXT("window.init");

    glfwSetErrorCallback(glfw_error_callback);

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

    window = glfwCreateWindow(width, height, "Gir1", nullptr, nullptr);
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
        std::exit(ERROR_GLEW_FAILED);
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    GL_CHECK_ERRORS;
    LOG_DEBUG << "GL_DEPTH_TEST enabled";
}

void Application::init_imgui() const {
    LOG_CONTEXT("imgui.init");

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
