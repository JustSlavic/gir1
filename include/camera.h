#ifndef GIR1_CAMERA_H
#define GIR1_CAMERA_H

#include <glm/glm.hpp>

struct Camera {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;

    float pitch;
    float yaw;

    float speed = 4;
    float rotation_speed = 40;

    bool cached = true;

    Camera();

    void translate_forward(float dt);
    void translate_backwards(float dt);
    void translate_right(float dt);
    void translate_left(float dt);
    void translate_up(float dt);
    void translate_down(float dt);

    void rotate_up(float dt);
    void rotate_down(float dt);
    void rotate_right(float dt);
    void rotate_left(float dt);

    void update();

    glm::mat4 get_view_matrix();
};

#endif //GIR1_CAMERA_H
