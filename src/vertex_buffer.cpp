#include <vertex_buffer.h>
#include <defines.h>

#include <GL/glew.h>


VertexBuffer::VertexBuffer(const void* data, size_t size) {
    GL_CALL(glGenBuffers(1, &id)); // give me 1 buffer and store its id in &id
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id)); // select this buffer (I'm about to work with it)
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}


VertexBuffer::~VertexBuffer() {
    GL_CALL(glDeleteBuffers(1, &id));
}


void VertexBuffer::bind() {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
}


void VertexBuffer::unbind() {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
