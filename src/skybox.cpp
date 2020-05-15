#include <skybox.h>
#include <defines.h>

#include <stb/stb_image.h>

#include <GL/glew.h>

LOG_CONTEXT("Skybox");

Skybox::Skybox(const char *dir_path) {
    float skybox_vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &vertex_array_id); GL_CHECK_ERRORS;
    glBindVertexArray(vertex_array_id); GL_CHECK_ERRORS;

    glGenBuffers(1, &vertex_buffer_id); GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id); GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id); GL_CHECK_ERRORS;

    i32 width, height, bits_per_pixel;
    stbi_set_flip_vertically_on_load(false);

    const char *face_names[] = {
            "posx.jpg",
            "negx.jpg",
            "posy.jpg",
            "negy.jpg",
            "posz.jpg",
            "negz.jpg",
    };

    for (int i = 0; i < 6; ++i) {
        std::string path{dir_path};

        path += "/";
        path += face_names[i];

        unsigned char *buffer = stbi_load(path.c_str(), &width, &height, &bits_per_pixel, STBI_rgb);
        if (!buffer) {
            LOG_ERROR << "Cannot load texture image " << path.c_str();
            std::exit(1);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

        stbi_image_free(buffer);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Skybox::~Skybox() {
    glDeleteTextures(1, &texture_id);
}

void Skybox::bind(unsigned int slot) const {
    glBindVertexArray(vertex_array_id);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    GL_CHECK_ERRORS;
}

void Skybox::unbind() {
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    GL_CHECK_ERRORS;
}

