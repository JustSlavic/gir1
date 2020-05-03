#ifndef GIR1_VERTEX_BUFFER_H
#define GIR1_VERTEX_BUFFER_H

#include <defines.h>


struct VertexBuffer {
    unsigned int id;

    VertexBuffer(const void* data, size_t size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};


#endif // GIR1_VERTEX_BUFFER_H
