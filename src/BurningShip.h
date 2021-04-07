#include "Fractal.h"

#ifndef FRACTOID_BURNINGSHIP_H
#define FRACTOID_BURNINGSHIP_H


class BurningShip : public Fractal{
public:
    explicit BurningShip(int iterations, int escape, bool smooth);

private:
    cv::Vec3b function(double pRe, double pIm) override;
    int escape, smooth;
};


#endif
