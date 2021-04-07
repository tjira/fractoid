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

void Fractal::show(std::vector<int> resolution, std::complex<double> center, double zoom) {
    cv::Mat3b canvas(resolution[1], resolution[0]);
    auto start = std::chrono::high_resolution_clock::now();
    paint(canvas, center.real(), center.imag(), zoom);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Fractal was generated in " << elapsed.count() << " ms." << std::endl;
    double scale = std::min(1024.0/resolution[0], 768.0/resolution[1]);
    cv::resize(canvas, canvas, cv::Size(), scale, scale);
    cv::imshow("Fractal", canvas);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
