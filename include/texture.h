#ifndef GIR1_TEXTURE
#define GIR1_TEXTURE

#include <defines.h>


struct Texture {
    unsigned int id = 0;

    const char *filename{nullptr};
    unsigned char *buffer{nullptr};

    i32 width = 0;
    i32 height = 0;
    i32 bits_per_pixel = 0;

    explicit Texture(const char* filename);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
};


#endif // GIR1_TEXTURE

