#pragma once

#include "../lib/json/json.hpp"
#include <complex>

using json = nlohmann::json;

namespace std {
    template<typename T> void from_json(const json &j, std::complex<T>& c) { c.real(j.at(0)), c.imag(j.at(1)); };
}

namespace Defaults {
    static json density = R"({
        "iterations" : 80,
        "bailout" : 10,
        "samples" : 1000000,
        "seed" : 1
    })"_json;

    static json escape = R"({
        "iterations" : 64,
        "bailout" : 8,
        "smooth" : true
    })"_json;

    static json orbitrap = R"({
        "iterations" : 80,
        "bailout" : 100,
        "trap" : 5,
        "fill" : false
    })"_json;

    static json periodic = R"({
        "amplitude" : [ 8, 32 ],
        "phase" : [ 0, 6.28 ],
        "seed" : [ 1, 1 ]
    })"_json;

    static json linear = R"({
        "from" : [ 0, 0, 0 ],
        "to" : [ 255, 255, 255 ]
    })"_json;

    static json solid = R"({
        "rgb" : [ 255, 255, 255 ]
    })"_json;
}
