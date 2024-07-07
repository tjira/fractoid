#pragma once

#include <fstream>
#include <vector>
#include <tuple>

typedef unsigned char uchar;

class Image {
public:
    // constructor
    Image(int width, int height, const std::array<uchar, 3>& background);

    // pixet access
    std::tuple<uchar&, uchar&, uchar&> operator()(int i, int j); uchar& operator()(int i, int j, int k);

    // image properties getter
    int getHeight() const {return height;} int getWidth() const {return width;}

    // image exporter
    void save(const std::string& filename) const;

private:
    std::vector<uchar> data; int width, height;
};
