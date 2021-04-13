#include "Fractal.h"


Fractal::Fractal(int iterations, double height) {
    this->height = height;
    this->iterations = iterations;
    randu(*randomizer, cv::Scalar(0), cv::Scalar(1));
}

void Fractal::paint(cv::Mat canvas, double pRe, double pIm, double zoom) {
    double unit = height / canvas.rows / zoom;
    double origin[2] = {pRe - unit * canvas.cols / 2, pIm + unit * canvas.rows / 2};
    for (int row = 0; row < canvas.rows; row++) {
        for (int column = 0; column < canvas.cols; column++) {
            cv::Vec3b color = function(origin[0] + unit * column, -origin[1] + unit * row);
            canvas.ptr<cv::Vec3b>(row)[column] = color;
        }
    }
}
