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
