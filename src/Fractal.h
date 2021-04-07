#include <opencv2/opencv.hpp>

#define SMOOTH \
    double sn = (smooth && n < iterations) ? n + 1 - log2(log(zRe * zRe + zIm * zIm) / 2) : (double) n

#define COLOR \
    cv::Vec3b \
        ((int) ((sin(sn * (*randomizer).at<double>(0, 0) / 1.5 + M_PI_2 * (*randomizer).at<double>(0, 1)) + 1) * 127.5), \
        (int) ((sin(sn * (*randomizer).at<double>(1, 0) / 1.5 + M_PI_2 * (*randomizer).at<double>(1, 1)) + 1) * 127.5), \
        (int) ((sin(sn * (*randomizer).at<double>(2, 0) / 1.5 + M_PI_2 * (*randomizer).at<double>(2, 1)) + 1) * 127.5))

#ifndef FRACTOID_FRACTAL_H
#define FRACTOID_FRACTAL_H


class Fractal {
public:
    explicit Fractal(int iterations, double height);
    void show(std::vector<int> resolution, std::complex<double> center, double zoom);

protected:
    cv::Mat2d* randomizer = new cv::Mat2d(3, 2);
    int iterations;

private:
    void paint(cv::Mat canvas, double re, double im, double zoom);
    virtual cv::Vec3b function(double pRe, double pIm) {return cv::Vec3b(0, 0, 0);};
    double height;
};


#endif
