#include <QObject>
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
    explicit Fractal(std::function<cv::Vec3b(double, double)> *function, double height);
	void paint(cv::Mat canvas, double re, double im, double zoom) const;
    static Fractal BurningShip(int iterations, int escape, bool smooth);
	static Fractal Julia(int iterations, int escape, bool smooth, std::complex<double> c);
	static Fractal Mandelbrot(int iterations, int escape, bool smooth);
	static Fractal Manowar(int iterations, int escape, bool smooth);
	static Fractal Newton(int iterations);
	static Fractal Phoenix(int iterations, int escape, bool smooth, std::complex<double> c);

private:
	std::function<cv::Vec3b(double, double)> function;
    double height;
};

#endif
