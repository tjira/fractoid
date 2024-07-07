#include "../include/image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include <stb_image_write.h>
#pragma GCC diagnostic pop

Image::Image(int width, int height, const std::array<uchar, 3>& background) : data(3 * width * height, 0), width(width), height(height) {
    // skip the collor filling if the background is black
    if (background == std::array<uchar, 3>{0, 0, 0}) return;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data.at(3 * (width * i + j) + 0) = background.at(0);
            data.at(3 * (width * i + j) + 1) = background.at(1);
            data.at(3 * (width * i + j) + 2) = background.at(2);
        }
    }
}

std::tuple<uchar&, uchar&, uchar&> Image::operator()(int i, int j) {
    return {data.at(3 * (width * i + j)), data.at(3 * (width * i + j) + 1), data.at(3 * (width * i + j) + 2)};
}

uchar& Image::operator()(int i, int j, int k) {
    return data.at(3 * (width * i + j) + k);
}

void Image::save(const std::string& filename) const {
    // extract the path extension
    std::string extension = filename.substr(filename.find_last_of(".") + 1);

    // save the image in the format specified by the extension
    if (extension == "png") stbi_write_png(filename.c_str(), width, height, 3, data.data(), 3 * width);
    else if (extension == "jpg") stbi_write_jpg(filename.c_str(), width, height, 3, data.data(), 100);
    else if (extension == "bmp") stbi_write_bmp(filename.c_str(), width, height, 3, data.data());

    // throw an error if the extension is not known
    else throw std::runtime_error("SAVING FILE WITH THE EXTENSION '" + extension + "' IS NOT SUPPORTED");
}
