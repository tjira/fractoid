#include "Manowar.h"

Manowar::Manowar(int iterations, int escape, bool smooth)
: Fractal(iterations, 2.2) {
    this->escape = escape;
    this->smooth = smooth;
}

cv::Vec3b Manowar::function(double pRe, double pIm) {
    int n = 0;
    double zRe = pRe, zIm = pIm, zRePrev = pRe, zImPrev = pIm, zReTemp, zImTemp;
    while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
        zReTemp = zRe;
        zImTemp = zIm;
        zRe = zRe * zRe - zIm * zIm + zRePrev + pRe;
        zIm = 2 * zReTemp * zIm +zImPrev + pIm;
        zRePrev = zReTemp;
        zImPrev = zImTemp;
        n++;
    }
    SMOOTH; return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
}
