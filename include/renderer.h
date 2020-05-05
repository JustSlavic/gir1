#ifndef GIR1_RENDERER_H
#define GIR1_RENDERER_H

#include <vertex_array.h>
#include <index_buffer.h>
#include <shader.h>


struct Renderer {
    static void init();
    static void clear();
    static void draw(const VertexArray &vertex_array, const IndexBuffer &index_buffer, Shader &shader);
    static void draw(const VertexArray &vertex_array, Shader &shader);
};


#endif // GIR1_RENDERER_H
