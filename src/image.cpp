#include "../include/image.h"
#include <stb_image_write.h>

Image::Image(int width, int height, const std::array<uchar, 3>& background) : data(3 * width * height, 0), width(width), height(height) {
    if (background.at(0) + background.at(1) + background.at(2) != 0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                data.at(3 * (width * i + j) + 0) = background.at(0);
                data.at(3 * (width * i + j) + 1) = background.at(1);
                data.at(3 * (width * i + j) + 2) = background.at(2);
            }
        }
    }
}

std::tuple<uchar&, uchar&, uchar&> Image::operator()(int i, int j) {
    return { data.at(3 * (width * i + j)), data.at(3 * (width * i + j) + 1), data.at(3 * (width * i + j) + 2) };
}

uchar& Image::operator()(int i, int j, int k) {
    return data.at(3 * (width * i + j) + k);
}

void Image::save(const std::string& filename) const {
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    if (extension == "png") {
        stbi_write_png(filename.c_str(), width, height, 3, data.data(), 3 * width);
    } else if (extension == "jpg") {
        stbi_write_jpg(filename.c_str(), width, height, 3, data.data(), 100);
    } else if (extension == "bmp") {
        stbi_write_bmp(filename.c_str(), width, height, 3, data.data());
    } else {
        throw std::runtime_error("Unknown file extension.");
    }
}
