#include <input.h>
#include <defines.h>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

#define HANDLE_KEY_PRESS(KEY) \
    if (key == GLFW_KEY_ ## KEY && action == GLFW_PRESS) input. ## KEY ## _pressed = true

#define HANDLE_KEY_FN(KEY, ACTION, CALL) \
    if (key == GLFW_KEY_ ## KEY && action == GLFW_ ## ACTION) CALL

#define PRINT_PRESSED(KEY) printf("Pressed " STRINGIFY(KEY) "\n")

    auto& input = KeyboardState::instance();

    HANDLE_KEY_FN(W, PRESS, input.W_pressed = true);
    HANDLE_KEY_FN(W, RELEASE, input.W_pressed = false);
    HANDLE_KEY_FN(S, PRESS, input.S_pressed = true);
    HANDLE_KEY_FN(S, RELEASE, input.S_pressed = false);
    HANDLE_KEY_FN(A, PRESS, input.A_pressed = true);
    HANDLE_KEY_FN(A, RELEASE, input.A_pressed = false);
    HANDLE_KEY_FN(D, PRESS, input.D_pressed = true);
    HANDLE_KEY_FN(D, RELEASE, input.D_pressed = false);

    HANDLE_KEY_FN(R, PRESS, input.R_pressed = true);
    HANDLE_KEY_FN(R, RELEASE, input.R_pressed = false);
    HANDLE_KEY_FN(F, PRESS, input.F_pressed = true);
    HANDLE_KEY_FN(F, RELEASE, input.F_pressed = false);

    HANDLE_KEY_FN(Z, PRESS, input.Z_pressed = true);
    HANDLE_KEY_FN(Z, RELEASE, input.Z_pressed = false);
    HANDLE_KEY_FN(X, PRESS, input.X_pressed = true);
    HANDLE_KEY_FN(X, RELEASE, input.X_pressed = false);

    HANDLE_KEY_FN(LEFT, PRESS, input.LEFT_pressed = true);
    HANDLE_KEY_FN(LEFT, RELEASE, input.LEFT_pressed = false);
    HANDLE_KEY_FN(RIGHT, PRESS, input.RIGHT_pressed = true);
    HANDLE_KEY_FN(RIGHT, RELEASE, input.RIGHT_pressed = false);
    HANDLE_KEY_FN(UP, PRESS, input.UP_pressed = true);
    HANDLE_KEY_FN(UP, RELEASE, input.UP_pressed = false);
    HANDLE_KEY_FN(DOWN, PRESS, input.DOWN_pressed = true);
    HANDLE_KEY_FN(DOWN, RELEASE, input.DOWN_pressed = false);

    HANDLE_KEY_FN(F3, PRESS, input.F3_pressed = true);
    HANDLE_KEY_FN(F3, RELEASE, input.F3_pressed = false);

    HANDLE_KEY_FN(ESCAPE, PRESS, std::exit(0));

#undef PRINT_PRESSED
#undef HANDLE_KEY_FN
#undef HANDLE_KEY_PRESS

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
