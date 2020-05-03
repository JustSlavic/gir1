#ifndef GIR1_VERTEX_BUFFER_LAYOUT_H
#define GIR1_VERTEX_BUFFER_LAYOUT_H


#include <vector>

#include <GL/glew.h>

#include <defines.h>


struct LayoutElement {
    unsigned int count;
    unsigned int type;
    unsigned int normalized;

    static unsigned int size_of_gl_type(unsigned int type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(GLfloat);
            case GL_UNSIGNED_INT:
                return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:
                return sizeof(GLbyte);
            default:
                ASSERT(false);
        }
    }
};


struct VertexBufferLayout {
    std::vector<LayoutElement> elements;
    unsigned int stride = 0;

    template <typename T>
    void push(unsigned int count) {
        ASSERT(false);
    }

    inline unsigned int get_stride() const { return stride; }
};

template <>
void VertexBufferLayout::push<float>(unsigned int count);

template <>
void VertexBufferLayout::push<unsigned int>(unsigned int count);

template <>
void VertexBufferLayout::push<unsigned char>(unsigned int count);

#endif // GIR1_VERTEX_BUFFER_LAYOUT_H
