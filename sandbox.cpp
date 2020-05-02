#include <iostream>

#include <boost/gil/extension/io/png.hpp>


int main() {
    std::cout << "Hello world" << std::endl;

    {
        using namespace boost::gil;
        rgb8_image_t image;
        read_image("resources/textures/wall.png", image, png_tag());

        std::stringstream buffer;
        write_view(buffer, view(image), png_tag());

        std::string bytes = buffer.str();

        const char* data = bytes.c_str();

        std::cout << data << std::endl;
    }

    return 0;
}
