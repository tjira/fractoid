#include "Mandelbrot.h"


Mandelbrot::Mandelbrot(int iterations, int escape, bool smooth)
: Fractal(iterations, 2.4) {
    this->escape = escape;
    this->smooth = smooth;
}

cv::Vec3b Mandelbrot::function(double pRe, double pIm) {
    int n = 0;
    double zRe = 0, zIm = 0, zReTemp;
    while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
        zReTemp = zRe;
        zRe = zRe * zRe - zIm * zIm + pRe;
        zIm = 2 * zReTemp * zIm + pIm;
        n++;
    }
    SMOOTH; return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
}
