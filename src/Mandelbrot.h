#include "Fractal.h"

#ifndef FRACTOID_MANDELBROT_H
#define FRACTOID_MANDELBROT_H


class Mandelbrot : public Fractal{
public:
    explicit Mandelbrot(int iterations, int escape, bool smooth);

private:
    cv::Vec3b function(double pRe, double pIm) override;
    int escape, smooth;
};


#endif
