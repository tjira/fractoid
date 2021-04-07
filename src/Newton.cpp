#include "Newton.h"


Newton::Newton(int iterations)
: Fractal(iterations, 3) {
    this->epsilon = 0.001;
}

cv::Vec3b Newton::function(double pRe, double pIm) {
    int n = 0;
    double zRe = pRe, zIm = pIm, zReTemp, denominator;
    while (n < iterations && ((zRe - 1) * (zRe - 1) + zIm * zIm > epsilon) && ((zRe + 0.5) * (zRe + 0.5) + (zIm + sqrt(3) / 2) * (zIm + sqrt(3) / 2) > epsilon) && ((zRe + 0.5) * (zRe + 0.5) + (zIm - sqrt(3) / 2) * (zIm - sqrt(3) / 2) > epsilon)) {
        zReTemp = zRe;
        denominator = 3 * (zRe * zRe + zIm * zIm) * (zRe * zRe + zIm * zIm);
        zRe = zRe - ((-zRe * zRe + zRe * zRe * zRe * zRe * zRe + zIm * zIm + 2 * zRe * zRe * zRe * zIm * zIm + zIm * zIm * zIm * zIm * zRe) / denominator);
        zIm = zIm - (2 * zReTemp * zIm + zReTemp * zReTemp * zReTemp * zReTemp * zIm + 2 * zReTemp * zReTemp * zIm * zIm * zIm + zIm * zIm * zIm * zIm * zIm) / denominator;
        n++;
    }
    auto sn = (double) n;
    return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
}
