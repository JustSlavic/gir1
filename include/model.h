#ifndef GIR1_MODEL_H
#define GIR1_MODEL_H

#include <glm/glm.hpp>
#include <vertex_array.h>
#include <vertex_buffer.h>
#include <vertex_buffer_layout.h>
#include <texture.h>


struct Cube {
    std::vector<float> vertices;

    VertexArray vertex_array;
    VertexBuffer *vertex_buffer;
    VertexBufferLayout layout;
//    Texture texture;

    glm::vec3 position;

    explicit Cube(glm::vec3 position);
};

#endif //GIR1_MODEL_H
