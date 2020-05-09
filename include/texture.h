#ifndef GIR1_TEXTURE_H
#define GIR1_TEXTURE_H


struct Texture {
    unsigned int id = 0;

    explicit Texture(const char* filename);
    ~Texture();
    Texture (const Texture&) = delete;

    void bind(unsigned int slot = 0) const;
    static void unbind() ;
};


#endif // GIR1_TEXTURE_H

