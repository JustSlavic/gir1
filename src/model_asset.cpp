#include <model_asset.h>
#include <fstream>


ModelAsset ModelAsset::load_my_model(const char *filename) {
    ModelAsset asset;

    std::ifstream input(filename);

    unsigned int size, stride, vertex_coord_len, normal_vec_len, texture_coord_len;
    input >> size >> stride >> vertex_coord_len >> normal_vec_len >> texture_coord_len;

    auto *vertices = new float[size * stride];

    for (unsigned i = 0; i < size * stride; ++i) input >> vertices[i];

    asset.vertex_array = new VertexArray;
    asset.vertex_buffer = new VertexBuffer(vertices, size * stride * sizeof(float));

    asset.vertex_buffer_layout = new VertexBufferLayout;
    asset.vertex_buffer_layout->push<float>(vertex_coord_len);
    asset.vertex_buffer_layout->push<float>(normal_vec_len);
    asset.vertex_buffer_layout->push<float>(texture_coord_len);

    asset.vertex_array->add_buffer(*asset.vertex_buffer, *asset.vertex_buffer_layout);

    delete[] vertices;

    return asset;
}
