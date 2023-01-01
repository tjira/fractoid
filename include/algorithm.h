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
        int iterations; double bailout; int samples; long seed;
    };
    struct Escape {
        int iterations; double bailout; bool smooth;
    };
    struct Orbitrap {
        double distance(std::complex<double> p) const;
        int iterations; double bailout; int trap; bool fill;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Density, iterations, bailout, samples, seed)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Escape, iterations, bailout, smooth)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Orbitrap, iterations, bailout, trap, fill)
};
