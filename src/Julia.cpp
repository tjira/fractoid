#include "Julia.h"


Julia::Julia(int iterations, int escape, bool smooth, std::complex<double> c)
: Fractal(iterations, 2.9) {
    this->escape = escape;
    this->smooth = smooth;
    this->cRe = c.real();
    this->cIm = c.imag();
}

cv::Vec3b Julia::function(double pRe, double pIm) {
    int n = 0;
    double zRe = pRe, zIm = pIm, zReTemp;
    while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
        zReTemp = zRe;
        zRe = zRe * zRe - zIm * zIm + cRe;
        zIm = 2 * zReTemp * zIm + cIm;
        n++;
    }
    SMOOTH; return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
}
