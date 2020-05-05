#include <camera.h>

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera()
    : position(glm::vec3(0.0f, 0.0f, -4.0f))
    , direction(glm::vec3(0.0f, 0.0f, 1.0f))
    , right(glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction)))
    , up(glm::normalize(glm::cross(direction, right)))
    , pitch(0.0f)
    , yaw(90.0f)
{}

void Camera::translate_forward(float dt) {
    position += speed * dt * direction;
}

void Camera::translate_backwards(float dt) {
    position -= speed * dt * direction;
}

void Camera::translate_right(float dt) {
    position -= speed * dt * right;
}

void Camera::translate_left(float dt) {
    position += speed * dt * right;
}

void Camera::translate_up(float dt) {
    position += speed * dt * up;
}

void Camera::translate_down(float dt) {
    position -= speed * dt * up;
}

void Camera::rotate_up(float dt) {
    pitch -= rotation_speed * dt;
    cached = false;
}

void Camera::rotate_down(float dt) {
    pitch += rotation_speed * dt;
    cached = false;
}

void Camera::rotate_left(float dt) {
    yaw -= rotation_speed * dt;
    cached = false;
}

void Camera::rotate_right(float dt) {
    yaw += rotation_speed * dt;
    cached = false;
}

void Camera::update() {
    right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
    up = glm::normalize(glm::cross(direction, right));

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction_;
    direction_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction_.y = sin(glm::radians(pitch));
    direction_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction_);

    cached = true;
}

glm::mat4 Camera::get_view_matrix() {
    if (!cached) { update(); }

    return glm::lookAt(position, position + direction, up);
}

