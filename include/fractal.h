#pragma once

#define ABS(X) ((X) < 0 ? -(X) : (X))

#define BUFFALOF     z = {ABS(z.real()), ABS(z.imag())}; z = z * z - z + p
#define BURNINGSHIPF z = {ABS(z.real()), ABS(z.imag())}; z = z * z + p
#define JULIAF       z = z * z + fractal.c
#define MANDELBROTF  z = z * z + p
#define MANOWARF     tmp = z; z = z * z + zp + p; zp = tmp
#define PHOENIXF     tmp = z; z = z * z - 0.5 * zp + 0.5667; zp = tmp

#define BUFFALO(ALG)     z = 0; ALG(BUFFALOF)
#define BURNINGSHIP(ALG) z = 0; ALG(BURNINGSHIPF)
#define JULIA(ALG)       z = p; ALG(JULIAF)
#define MANDELBROT(ALG)  z = 0; ALG(MANDELBROTF)
#define MANOWAR(ALG)     z = p, zp = p; ALG(MANOWARF)
#define PHOENIX(ALG)     z = p, zp = fractal.c; ALG(PHOENIXF)

namespace Fractal {
    struct Buffalo {};
    struct BurningShip {};
    struct Julia {
        std::complex<double> c{0, 1};
    };
    struct Mandelbrot {};
    struct Manowar {};
    struct Phoenix {
        std::complex<double> c{0, 0};
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Julia, c);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Phoenix, c);
};
