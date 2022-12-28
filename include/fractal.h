#include <complex>

#define ABS(X) ((X) < 0 ? -(X) : (X))

#define BUFFALOF z = { ABS(z.real()), ABS(z.imag()) }; z = z * z - z + p
#define BURNINGSHIPF z = { ABS(z.real()), ABS(z.imag()) }; z = z * z + p
#define JULIAF z = z * z + fractal.c
#define MANDELBROTF z = z * z + p
#define MANOWARF tmp = z; z = z * z + zp + p; zp = tmp
#define PHOENIXF tmp = z; z = z * z - 0.5 * zp + 0.5667; zp = tmp

#define BUFFALO(ALG) z = 0; ALG(BUFFALOF)
#define BURNINGSHIP(ALG) z = 0; ALG(BURNINGSHIPF)
#define JULIA(ALG) z = p; ALG(JULIAF)
#define MANDELBROT(ALG) z = 0; ALG(MANDELBROTF)
#define MANOWAR(ALG) z = p, zp = p; ALG(MANOWARF)
#define PHOENIX(ALG) z = p, zp = fractal.c; ALG(PHOENIXF)

namespace Fractal {
    struct Julia { std::complex<double> c; };
    struct Phoenix { std::complex<double> c; };
    struct Buffalo {};
    struct BurningShip {};
    struct Mandelbrot {};
    struct Manowar {};
};
