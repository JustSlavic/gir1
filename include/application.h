#ifndef GIR1_APPLICATION_H
#define GIR1_APPLICATION_H

// opengl imports
#include <GLFW/glfw3.h>


struct Application {
    GLFWwindow *window;
    int width;
    int height;

    Application(int width, int height);

    void load_scene();
    void run();

private:
    void init_window(int width, int height);
    void init_imgui() const;
};

#endif //GIR1_APPLICATION_H
