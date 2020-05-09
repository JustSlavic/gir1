#include <texture.h>
#include <defines.h>
#include <utils.h>

#include <stb/stb_image.h>

#include <GL/glew.h>


Texture::Texture(const char *filename) {
    GL_CALL(glGenTextures(1, &id));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

    i32 width, height, bits_per_pixel;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *buffer = stbi_load(filename, &width, &height, &bits_per_pixel, STBI_rgb_alpha);

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));

    if (buffer) stbi_image_free(buffer);
}

Texture::~Texture() {
    glDeleteTextures(1, &id); GL_CHECK_ERRORS;
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot); GL_CHECK_ERRORS;
    glBindTexture(GL_TEXTURE_2D, id); GL_CHECK_ERRORS;
}

void Texture::unbind() {
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}
