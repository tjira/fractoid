#include "../lib/json/json.hpp"
#include <complex>

#define ESCAPE(FC) for(int n=0;n<alg.iterations;n++){FC;if(std::norm(z)>alg.bailout*alg.bailout){value=n;break;}}
#define ORBIT(FC) for(int n=0;n<alg.iterations;n++){FC;if(std::norm(z)>alg.bailout*alg.bailout){break;}orbit.push_back(z);}

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

double Algorithm::Orbitrap::distance(std::complex<double> p) const {
    if (trap == 1) return std::norm(p);
    else if (trap == 2) return std::min(std::abs(p.real()), std::abs(p.imag()));
    else if (trap == 3) return 0.70710678f * std::min(std::abs(p.real() - p.imag()), std::abs(p.real() + p.imag()));
    else if (trap == 4) return std::abs(std::norm(p) - 1.0);
    else if (trap == 5) return std::min(std::norm(p), std::abs(std::norm(p) - 1.0));
    else throw std::runtime_error("Unknown trap number.");
}
