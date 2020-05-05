#include <input.h>
#include <defines.h>

#include <cstdio>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

#define HANDLE_KEY(KEY, ACTION, CALL) \
    if (key == GLFW_KEY_ ## KEY && action == GLFW_ ## ACTION) CALL

#define PRINT_PRESSED(KEY) printf("Pressed " STRINGIFY(KEY) "\n")

    auto& input = KeyboardState::instance();

    HANDLE_KEY(W, PRESS, input.W_pressed = true);
    HANDLE_KEY(W, RELEASE, input.W_pressed = false);
    HANDLE_KEY(S, PRESS, input.S_pressed = true);
    HANDLE_KEY(S, RELEASE, input.S_pressed = false);
    HANDLE_KEY(A, PRESS, input.A_pressed = true);
    HANDLE_KEY(A, RELEASE, input.A_pressed = false);
    HANDLE_KEY(D, PRESS, input.D_pressed = true);
    HANDLE_KEY(D, RELEASE, input.D_pressed = false);

    HANDLE_KEY(R, PRESS, input.R_pressed = true);
    HANDLE_KEY(R, RELEASE, input.R_pressed = false);
    HANDLE_KEY(F, PRESS, input.F_pressed = true);
    HANDLE_KEY(F, RELEASE, input.F_pressed = false);

    HANDLE_KEY(Z, PRESS, input.Z_pressed = true);
    HANDLE_KEY(Z, RELEASE, input.Z_pressed = false);
    HANDLE_KEY(X, PRESS, input.X_pressed = true);
    HANDLE_KEY(X, RELEASE, input.X_pressed = false);

    HANDLE_KEY(LEFT, PRESS, input.LEFT_pressed = true);
    HANDLE_KEY(LEFT, RELEASE, input.LEFT_pressed = false);
    HANDLE_KEY(RIGHT, PRESS, input.RIGHT_pressed = true);
    HANDLE_KEY(RIGHT, RELEASE, input.RIGHT_pressed = false);
    HANDLE_KEY(UP, PRESS, input.UP_pressed = true);
    HANDLE_KEY(UP, RELEASE, input.UP_pressed = false);
    HANDLE_KEY(DOWN, PRESS, input.DOWN_pressed = true);
    HANDLE_KEY(DOWN, RELEASE, input.DOWN_pressed = false);

    HANDLE_KEY(ESCAPE, PRESS, std::exit(0));

#undef HANDLE_KEY
#undef PRINT_PRESSED

}

void cursor_position_callback(GLFWwindow* window, double x, double y) {
    // get previous position
    auto& input = KeyboardState::instance();

    input.cursor_dx = x - input.cursor_x;
    input.cursor_dy = y - input.cursor_y;

    if (input.LMB_pressed) {
        input.LMB_drag_x = input.cursor_dx;
        input.LMB_drag_y = input.cursor_dy;
    }
    if (input.RMB_pressed) {
        input.RMB_drag_x = input.cursor_dx;
        input.RMB_drag_y = input.cursor_dy;
    }

    // store new positions
    input.cursor_x = x;
    input.cursor_y = y;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto& input = KeyboardState::instance();

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) input.LMB_pressed = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) input.LMB_pressed = false;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) input.RMB_pressed = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) input.RMB_pressed = false;
}

KeyboardState &KeyboardState::instance() {
    static KeyboardState instance;
    return instance;
}
