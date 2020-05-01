#include <index_buffer.h>
#include <defines.h>

#include <GL/glew.h>


IndexBuffer::IndexBuffer(const unsigned int* data, size_t count) :count(count) {
    static_assert(sizeof(unsigned int) == sizeof(GLuint), "oops");

    GL_CALL(glGenBuffers(1, &id)); // give me 1 buffer and store its id in &id
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id)); // select this buffer (I'm about to work with it)
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
}


IndexBuffer::~IndexBuffer() {
    GL_CALL(glDeleteBuffers(1, &id));
}


void IndexBuffer::bind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}


void IndexBuffer::unbind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
