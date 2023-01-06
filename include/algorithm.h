#pragma once

#include "../lib/json/json.hpp"
#include <complex>

#define ESCAPE(FC) for(int n=0;n<alg.iterations;n++){FC;if(std::norm(z)>alg.bailout*alg.bailout){value=n;break;}}
#define ORBIT(FC) for(int n=0;n<alg.iterations;n++){FC;if(std::norm(z)>alg.bailout*alg.bailout){break;}orbit.push_back(z);}

#define ORBITRAP_TRAP_1(P) std::norm(p)
#define ORBITRAP_TRAP_2(P) std::min(std::abs(p.real()), std::abs(p.imag()))
#define ORBITRAP_TRAP_3(P) 0.70710678f * std::min(std::abs(p.real() - p.imag()), std::abs(p.real() + p.imag()))
#define ORBITRAP_TRAP_4(P) std::abs(std::norm(p) - 1.0)
#define ORBITRAP_TRAP_5(P) std::min(std::norm(p), std::abs(std::norm(p) - 1.0))

#define DISTANCE(P) \
    alg.trap == 1 ? ORBITRAP_TRAP_1(P) : \
    alg.trap == 2 ? ORBITRAP_TRAP_2(P) : \
    alg.trap == 3 ? ORBITRAP_TRAP_3(P) : \
    alg.trap == 4 ? ORBITRAP_TRAP_4(P) : \
    alg.trap == 5 ? ORBITRAP_TRAP_5(P) : 0

namespace Algorithm {
    struct Density {
        int iterations; float bailout; int samples; long seed;
    };
    struct Escape {
        static std::function<std::string(std::string, std::string)> code;
        int iterations; float bailout; bool smooth;
    };
    struct Orbitrap {
        static std::function<std::string(std::string, std::string)> code;
        int iterations; float bailout; int trap; bool fill;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Density, iterations, bailout, samples, seed)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Escape, iterations, bailout, smooth)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Orbitrap, iterations, bailout, trap, fill)
};

inline std::function<std::string(std::string, std::string)> Algorithm::Escape::code = [](std::string init, std::string code) { return R"(
    float function(float pRe, float pIm) {)"+init+R"(
        for(int n = 0; n < alg.iters; n++){)"+code+R"(
            if(norm(zRe, zIm) > alg.bail * alg.bail) {
                return n - (alg.smoothing ? log2(0.5 * log(norm(zRe, zIm))) : 0);
            }
        }
        return alg.iters;
    }
)";};

inline std::function<std::string(std::string, std::string)> Algorithm::Orbitrap::code = [](std::string init, std::string code) { return R"(
    float function(float pRe, float pIm) {)"+init+R"(float minDistance = alg.iters;
        for(int n = 0; n < alg.iters; n++){)"+code+R"(
            if(norm(zRe, zIm) > alg.bail * alg.bail) {
                return minDistance;
            }
            float distance;
            if (alg.trap == 1) distance = norm(zRe, zIm);
            else if (alg.trap == 2) distance = min(abs(zRe), abs(zIm));
            else if (alg.trap == 3) distance = 0.70710678f * min(abs(zRe - zIm), abs(zRe + zIm));
            else if (alg.trap == 4) distance = abs(norm(zRe, zIm) - 1);
            else if (alg.trap == 5) distance = min(norm(zRe, zIm), abs(norm(zRe, zIm) - 1));
            if (distance < minDistance) minDistance = distance;
        }
        return alg.fill ? alg.iters : minDistance;
    }
)";};
