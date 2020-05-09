#ifndef GIR1_RENDERER_H
#define GIR1_RENDERER_H

#include <model.h>
#include <vertex_array.h>
#include <index_buffer.h>
#include <shader.h>
#include <model_instance.h>
#include <skybox.h>


struct Renderer {
    static void init();
    static void clear();
    static void draw(const VertexArray &vertex_array, const IndexBuffer &index_buffer, Shader &shader);
    static void draw(const VertexArray &vertex_array, Shader &shader);
    static void draw(const Cube &cube, Shader &shader);
    static void draw(const ModelInstance &model);
    static void draw(const Skybox& skybox);
};


#endif // GIR1_RENDERER_H
