#include <vertex_buffer_layout.h>


template <>
void VertexBufferLayout::push<float>(unsigned int count) {
    elements.push_back({count, GL_FLOAT, GL_FALSE});
    stride += count*LayoutElement::size_of_gl_type(GL_FLOAT);
}

template <>
void VertexBufferLayout::push<unsigned int>(unsigned int count) {
    elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
    stride += count*LayoutElement::size_of_gl_type(GL_UNSIGNED_INT);
}

template <>
void VertexBufferLayout::push<unsigned char>(unsigned int count) {
    elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
    stride += count*LayoutElement::size_of_gl_type(GL_UNSIGNED_BYTE);
}
