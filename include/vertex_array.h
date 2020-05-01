#ifndef GIR1_VERTEX_ARRAY
#define GIR1_VERTEX_ARRAY


#include <vertex_buffer.h>
#include <vertex_buffer_layout.h>


struct VertexArray {
    unsigned int id;

    VertexArray();
    ~VertexArray();

    void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

    void bind() const;
    void unbind() const;
};


#endif // GIR1_VERTEX_ARRAY
