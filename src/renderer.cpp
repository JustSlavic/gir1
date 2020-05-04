#include <renderer.h>

void Renderer::clear() {
//    GL_CALL(glClearColor(0.2, 0.2, 0.2, 1.0));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray &vertex_array, const IndexBuffer &index_buffer, Shader &shader) {
    vertex_array.bind();
    index_buffer.bind();
    shader.bind();

    GL_CALL(glDrawElements(GL_TRIANGLES, index_buffer.count, GL_UNSIGNED_INT, nullptr));
}

void Renderer::init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
