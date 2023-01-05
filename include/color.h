#include "../lib/json/json.hpp"

typedef unsigned char uchar;

namespace Color {
    // Linear coloring
    struct Linear {
        std::tuple<uchar, uchar, uchar> get(double v) const {
            uchar r = from.at(0) + v * (to.at(0) - from.at(0));
            uchar g = from.at(1) + v * (to.at(1) - from.at(1));
            uchar b = from.at(2) + v * (to.at(2) - from.at(2));
            return { r, g, b };
        };
        std::array<int, 3> from, to;
    };

    // Periodic coloring
    struct Periodic {
        std::tuple<uchar, uchar, uchar> get(double v) const {
            uchar r = (std::sin(r1 * v + r2) + 1) * 127.5;
            uchar g = (std::sin(g1 * v + g2) + 1) * 127.5;
            uchar b = (std::sin(b1 * v + b2) + 1) * 127.5;
            return { r , g, b };
        };
        std::array<double, 2> amplitude, phase;
        double r1, g1, b1, r2, g2, b2;
        std::array<long, 2> seed;
    };

    // Solid coloring
    struct Solid {
        std::tuple<uchar, uchar, uchar> get(double) const {
            return { (uchar)rgb.at(0), (uchar)rgb.at(1), (uchar)rgb.at(2) };
        }
        std::array<int, 3> rgb;
    };

    // Define json loaders
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Linear, from, to)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Periodic, amplitude, phase, seed)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Solid, rgb)
};
