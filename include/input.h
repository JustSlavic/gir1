#ifndef GIR1_INPUT_H
#define GIR1_INPUT_H

#include <GLFW/glfw3.h>

struct KeyboardState {

};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

#endif // GIR1_INPUT_H
