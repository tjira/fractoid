#pragma once

#include "../lib/json/json.hpp"
#include <complex>

#define BACKGROUND 0, 0, 0
#define OUTPUT "fractal.png"
#define RESOLUTION 1280, 720

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

    static json patch(json input) {
        std::string algorithm = input.at("algorithm").at("name"), color = input.at("color").at("name");
        if (algorithm == "density") Defaults::density.merge_patch(input.at("algorithm")), input.at("algorithm") = Defaults::density;
        if (algorithm == "escape") Defaults::escape.merge_patch(input.at("algorithm")), input.at("algorithm") = Defaults::escape;
        if (algorithm == "orbitrap") Defaults::orbitrap.merge_patch(input.at("algorithm")), input.at("algorithm") = Defaults::orbitrap;
        if (color == "linear") Defaults::linear.merge_patch(input.at("color")), input.at("color") = Defaults::linear;
        if (color == "periodic") Defaults::periodic.merge_patch(input.at("color")), input.at("color") = Defaults::periodic;
        if (color == "solid") Defaults::solid.merge_patch(input.at("color")), input.at("color") = Defaults::solid;
        return input;
    }
}
