#pragma once

#include "defaults.h"
#include "../lib/json/json.hpp"
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
    struct ShaderCode {
        std::string init, code;
    };
    struct Buffalo {
        inline static ShaderCode code = {
            "float zRe = 0, zIm = 0;", R"(
                float zReTemp = zRe;
                zRe = zRe * zRe - zIm * zIm - abs(zRe) + pRe;
                zIm = 2 * abs(zReTemp * zIm) - abs(zIm) + pIm;
            )"
        };
    };
    struct BurningShip {
        inline static ShaderCode code = {
            "float zRe = 0, zIm = 0;", R"(
                float zReTemp = zRe;
                zRe = zRe * zRe - zIm * zIm + pRe;
                zIm = 2 * abs(zReTemp * zIm) + pIm;
            )"
        };
    };
    struct Julia {
        inline static ShaderCode code = {
            "float zRe = pRe, zIm = pIm;", R"(
                float zReTemp = zRe;
                zRe = zRe * zRe - zIm * zIm + fractal.cRe;
                zIm = 2 * zReTemp * zIm + fractal.cIm;
            )"
        };
        std::complex<double> c{ 0, 1 };
    };
    struct Mandelbrot {
        inline static ShaderCode code = {
            "float zRe = 0, zIm = 0;", R"(
                float zReTemp = zRe;
                zRe = zRe * zRe - zIm * zIm + pRe;
                zIm = 2 * zReTemp * zIm + pIm;
            )"
        };
    };
    struct Manowar {
        inline static ShaderCode code = {
            "float zRe = pRe, zIm = pIm, zpRe = pRe, zpIm = pIm;", R"(
                float zReTemp = zRe, zImTemp = zIm;
                zRe = zRe * zRe - zIm * zIm + zpRe + pRe;
                zIm = 2 * zReTemp * zIm + zpIm + pIm;
                zpRe = zReTemp, zpIm = zImTemp;
            )"
        };
    };
    struct Phoenix {
        inline static ShaderCode code = {
            "float zRe = -pIm; float zIm = pRe, zpRe = fractal.cRe, zpIm = fractal.cIm;", R"(
                float zReTemp = zRe, zImTemp = zIm;
                zRe = zRe * zRe - zIm * zIm - 0.5f * zpRe + 0.5667f;
                zIm = 2 * zReTemp * zIm - 0.5f * zpIm;
                zpRe = zReTemp, zpIm = zImTemp;
            )"
        };
        std::complex<double> c{ 0, 0 };
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Julia, c);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Phoenix, c);
};
