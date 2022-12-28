#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <array>
#include <fstream>
#include <tuple>
#include <vector>

typedef unsigned char uchar;

class Image {
public:
    Image(int width, int height, const std::array<uchar, 3>& background);
    std::tuple<uchar&, uchar&, uchar&> operator()(int i, int j);
    uchar& operator()(int i, int j, int k);
    int getHeight() const { return height; };
    int getWidth() const { return width; };
    void save(const std::string& filename) const;

private:
    std::vector<uchar> data;
    int width, height;
};
