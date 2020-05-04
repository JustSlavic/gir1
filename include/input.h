#ifndef GIR1_INPUT_H
#define GIR1_INPUT_H

#include <GLFW/glfw3.h>

struct KeyboardState {
    double cursor_x = 0;
    double cursor_y = 0;
    bool LMB_pressed = false;
    bool RMB_pressed = false;

    double LMB_drag_x = 0;
    double LMB_drag_y = 0;
    double RMB_drag_x = 0;
    double RMB_drag_y = 0;

    static KeyboardState& instance();
private:
    KeyboardState() = default;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

#endif // GIR1_INPUT_H
