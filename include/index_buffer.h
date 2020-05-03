#ifndef GIR1_INDEX_BUFFER_H
#define GIR1_INDEX_BUFFER_H

#include <defines.h>


struct IndexBuffer {
    unsigned int id;
    unsigned int count;

    IndexBuffer(const unsigned int* data, size_t count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
};


#endif // GIR1_INDEX_BUFFER_H
