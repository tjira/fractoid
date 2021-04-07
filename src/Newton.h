#include "Fractal.h"

#ifndef FRACTOID_NEWTON_H
#define FRACTOID_NEWTON_H


class Newton : public Fractal{
public:
    explicit Newton(int iterations);

private:
    cv::Vec3b function(double pRe, double pIm) override;
    double epsilon;
};


#endif
