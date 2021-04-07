#include "Fractal.h"

#ifndef FRACTOID_MANOWAR_H
#define FRACTOID_MANOWAR_H


class Manowar : public Fractal{
public:
    explicit Manowar(int iterations, int escape, bool smooth);

private:
    cv::Vec3b function(double pRe, double pIm) override;
    int escape, smooth;
};


#endif
