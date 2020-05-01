#ifndef GIR1_VERTEX_BUFFER
#define GIR1_VERTEX_BUFFER

#include <defines.h>


struct VertexBuffer {
    unsigned int id;

    VertexBuffer(const void* data, size_t size);
    ~VertexBuffer();

    void bind();
    void unbind();
};


#endif // GIR1_VERTEX_BUFFER
