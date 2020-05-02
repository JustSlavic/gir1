#include <vertex_array.h>


VertexArray::VertexArray() {
    GL_CALL(glGenVertexArrays(1, &id));
}

VertexArray::~VertexArray() {
    GL_CALL(glDeleteVertexArrays(1, &id));
}

void VertexArray::add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout) const {
    bind();
    vb.bind();

    unsigned long offset = 0;

    for (unsigned int i = 0; i < layout.elements.size(); ++i) {
        auto& element = layout.elements[i];

        GL_CALL(glEnableVertexAttribArray(i));
        GL_CALL(glVertexAttribPointer(
                /*index*/ i,
                /*size of vertex*/ element.count,
                /*type*/ element.type,
                /*normalize?*/ element.normalized,
                /*stride*/ layout.get_stride(),
                /*pointer*/ (void*) offset));

        offset += element.count * LayoutElement::size_of_gl_type(element.type);
    }
}

void VertexArray::bind() const {
    GL_CALL(glBindVertexArray(id));
}

void VertexArray::unbind() const {
    GL_CALL(glBindVertexArray(0));
}
