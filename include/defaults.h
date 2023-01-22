#pragma once

#include "../lib/json/json.hpp"
#include <complex>

using json = nlohmann::json;

namespace std {
    template<typename T> void from_json(const json &j, std::complex<T>& c) { c.real(j.at(0)), c.imag(j.at(1)); };
    template< class T > void to_json(json &j, const std::complex< T > &p) { j = json{p.real(), p.imag()}; }
}

namespace Defaults {
    static json julia = R"({
        "c" : [ 0, 1 ]
    })"_json;

    static json phoenix = R"({
        "c" : [ 0, 0 ]
    })"_json;

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
        "amplitude" : [ 31.93, 30.38, 11.08 ],
        "phase" : [ 6.26, 5.86, 0.80 ]
    })"_json;

    static json linear = R"({
        "from" : [ 0, 0, 0 ],
        "to" : [ 255, 255, 255 ]
    })"_json;

    static json solid = R"({
        "rgb" : [ 255, 255, 255 ]
    })"_json;
}
