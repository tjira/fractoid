#include "Fractal.h"

#ifndef FRACTOID_PHOENIX_H
#define FRACTOID_PHOENIX_H


class Phoenix : public Fractal {
public:
    explicit Phoenix(int iterations, int escape, bool smooth, std::complex<double> c);

private:
    cv::Vec3b function(double pRe, double pIm) override;
    double cRe, cIm;
    int escape, smooth;
};


#endif
