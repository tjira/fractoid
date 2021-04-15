#include <QObject>
#include <opencv2/opencv.hpp>

#ifndef FRACTOID_FRACTAL_H
#define FRACTOID_FRACTAL_H

class Fractal : public QObject {
	Q_OBJECT
public:
    explicit Fractal(std::function<double(double, double)> *function);
    ~Fractal();
	void paint(cv::Mat canvas, double re, double im, double zoom);
    static Fractal BurningShip(int iterations, int escape, bool smooth);
	static Fractal Julia(int iterations, int escape, bool smooth, double cRe, double cIm);
	static Fractal Mandelbrot(int iterations, int escape, bool smooth);
	static Fractal Manowar(int iterations, int escape, bool smooth);
	static Fractal Phoenix(int iterations, int escape, bool smooth, double cRe, double cIm);

private:
	std::function<double(double, double)> function;

signals:
	void progress(int percentage);
};

#endif
