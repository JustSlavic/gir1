#include <input.h>
#include <defines.h>

#include <cstdio>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

#define HANDLE_KEY(KEY, ACTION, CALL) \
    if (key == GLFW_KEY_ ## KEY && action == GLFW_ ## ACTION) CALL

#define PRINT_PRESSED(KEY) printf("Pressed " STRINGIFY(KEY) "\n")

    HANDLE_KEY(W, PRESS, PRINT_PRESSED(W));
    HANDLE_KEY(S, PRESS, PRINT_PRESSED(S));
    HANDLE_KEY(A, PRESS, PRINT_PRESSED(A));
    HANDLE_KEY(D, PRESS, PRINT_PRESSED(D));

    HANDLE_KEY(LEFT, PRESS, PRINT_PRESSED(left));
    HANDLE_KEY(RIGHT, PRESS, PRINT_PRESSED(right));
    HANDLE_KEY(UP, PRESS, PRINT_PRESSED(up));
    HANDLE_KEY(DOWN, PRESS, PRINT_PRESSED(down));

    HANDLE_KEY(ESCAPE, PRESS, std::exit(0));

#undef HANDLE_KEY
#undef PRINT_PRESSED

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // get previous position
    auto& input = KeyboardState::instance();

    if (input.LMB_pressed) {
        input.LMB_drag_x = xpos - input.cursor_x;
        input.LMB_drag_y = ypos - input.cursor_y;
//        printf("LMB DRAG (%3.0lf, %3.0lf) pixels\n", xpos - input.cursor_x, ypos - input.cursor_y);
    }
    if (input.RMB_pressed) {
        input.RMB_drag_x = xpos - input.cursor_x;
        input.RMB_drag_y = ypos - input.cursor_y;
//        printf("RMB DRAG (%3.0lf, %3.0lf) pixels\n", xpos - input.cursor_x, ypos - input.cursor_y);
    }
    // if left button is pressed, change view matrix (translation)
    // if right button is pressed, change view matrix (rotation)

    // store new positions
    input.cursor_x = xpos;
    input.cursor_y = ypos;
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
