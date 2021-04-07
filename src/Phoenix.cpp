#include "Phoenix.h"


Phoenix::Phoenix(int iterations, int escape, bool smooth, std::complex<double> c)
: Fractal(iterations, 2.4) {
    this->escape = escape;
    this->smooth = smooth;
    this->cRe = c.real();
    this->cIm = c.imag();
}

cv::Vec3b Phoenix::function(double pRe, double pIm) {
    int n = 0;
    double zRe = -pIm, zIm = pRe, zRePrev = cRe, zImPrev = cIm, zReTemp, zImTemp;
    while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
        zReTemp = zRe;
        zImTemp = zIm;
        zRe = zRe * zRe - zIm * zIm + 0.56667 - 0.5 * zRePrev;
        zIm = 2 * zReTemp * zIm - 0.5 * zImPrev;
        zRePrev = zReTemp;
        zImPrev = zImTemp;
        n++;
    }
    SMOOTH; return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
}
