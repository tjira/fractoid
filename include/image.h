#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "defaults.h"

typedef unsigned char uchar;

class Image {
public:
    Image(int width, int height, const std::array<uchar, 3>& background);
    std::tuple<uchar&, uchar&, uchar&> operator()(int i, int j);
    void save(const std::string& filename) const;
    uchar& operator()(int i, int j, int k);
    int getHeight() const {return height;}
    int getWidth() const {return width;}

private:
    std::vector<uchar> data;
    int width, height;
};
