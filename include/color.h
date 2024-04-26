#pragma once

#include "defaults.h"

typedef unsigned char uchar;

namespace Color {
    struct Linear {
        static std::string code;
        std::tuple<uchar, uchar, uchar> get(double v) const {
            uchar r = from.at(0) + v * (to.at(0) - from.at(0));
            uchar g = from.at(1) + v * (to.at(1) - from.at(1));
            uchar b = from.at(2) + v * (to.at(2) - from.at(2));
            return { r, g, b };
        };
        std::array<int, 3> from, to;
    };
    struct Periodic {
        static std::string code;
        std::tuple<uchar, uchar, uchar> get(double v) const {
            uchar r = (std::sin(amplitude.at(0) * v + phase.at(0)) + 1) * 127.5;
            uchar g = (std::sin(amplitude.at(1) * v + phase.at(1)) + 1) * 127.5;
            uchar b = (std::sin(amplitude.at(2) * v + phase.at(2)) + 1) * 127.5;
            return { r , g, b };
        };
        std::array<double, 3> amplitude, phase;
    };
    struct Solid {
        static std::string code;
        std::tuple<uchar, uchar, uchar> get(double) const {
            return { (uchar)rgb.at(0), (uchar)rgb.at(1), (uchar)rgb.at(2) };
        }
        std::array<int, 3> rgb;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Linear, from, to)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Periodic, amplitude, phase)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Solid, rgb)
};
