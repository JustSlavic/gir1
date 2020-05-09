#ifndef GIR1_MODEL_ASSET_H
#define GIR1_MODEL_ASSET_H

#include <shader.h>
#include <texture.h>
#include <vertex_array.h>
#include <vertex_buffer.h>
#include <vertex_buffer_layout.h>


struct ModelAsset {
    Shader *shader{nullptr};
    Texture *texture{nullptr};

    VertexArray *vertex_array{nullptr};
    VertexBuffer *vertex_buffer{nullptr};
    VertexBufferLayout *vertex_buffer_layout{nullptr};

    static ModelAsset load_my_model(const char *filename);
};


#endif //GIR1_MODEL_ASSET_H
