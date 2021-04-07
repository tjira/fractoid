#include "Fractal.h"

#ifndef FRACTOID_JULIA_H
#define FRACTOID_JULIA_H


class Julia : public Fractal {
public:
    explicit Julia(int iterations, int escape, bool smooth, std::complex<double> c);

private:
    cv::Vec3b function(double pRe, double pIm) override;
    double cRe, cIm;
    int escape, smooth;
};


#endif
