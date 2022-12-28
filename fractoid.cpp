#include "include/image.h"
#include "include/painter.h"
#include "include/timer.h"
#include "include/uniform.h"
#include "lib/argparse/argparse.hpp"
#include <filesystem>

#define BACKGROUND 0, 0, 0
#define OUTPUT "fractal.png"
#define RESOLUTION 1280, 720

json density = R"({
    "iterations" : 80,
    "bailout" : 10,
    "samples" : 1000000,
    "seed" : 1
})"_json;

json escape = R"({
    "iterations" : 80,
    "bailout" : 10,
    "smooth" : true
})"_json;

json orbitrap = R"({
    "iterations" : 80,
    "bailout" : 10,
    "trap" : 5,
    "fill" : false
})"_json;

json periodic = R"({
    "amplitude" : [ 8, 32 ],
    "phase" : [ 0, 6.28 ],
    "seed" : [ 1, 1 ]
})"_json;

json linear = R"({
    "from" : [ 0, 0, 0 ],
    "to" : [ 255, 255, 255 ]
})"_json;

json solid = R"({
    "rgb" : [ 255, 255, 255 ]
})"_json;

namespace std {
    template<typename T> void from_json(const json &j, std::complex<T>& c) { c.real(j.at(0)), c.imag(j.at(1)); };
}

json patch(json input) {
    std::string algorithm = input.at("algorithm").at("name"), color = input.at("color").at("name");
    if (algorithm == "density") density.merge_patch(input.at("algorithm")), input.at("algorithm") = density;
    if (algorithm == "escape") escape.merge_patch(input.at("algorithm")), input.at("algorithm") = escape;
    if (algorithm == "orbitrap") orbitrap.merge_patch(input.at("algorithm")), input.at("algorithm") = orbitrap;
    if (color == "linear") linear.merge_patch(input.at("color")), input.at("color") = linear;
    if (color == "periodic") periodic.merge_patch(input.at("color")), input.at("color") = periodic;
    if (color == "solid") solid.merge_patch(input.at("color")), input.at("color") = solid;
    return input;
}

template<class F, class A, class C>
Image run(const json& input, const F& fractal, const A& alg, const C& col) {
    std::array<uchar, 3> background = input.value("background", std::array<uchar, 3>{ BACKGROUND });
    std::array<int, 2> resolution = input.value("resolution", std::array<int, 2>{ RESOLUTION });
    Painter painter(fractal, alg, col, { background, resolution });
    return painter.paint(input.at("center"), input.at("zoom"));
}

template<class F, class A>
Image run(const json& input, const F& fractal, const A& alg) {
    using namespace Color;
    if (input.at("color").at("name") == "linear") return run(input, fractal, alg, input.at("color").get<Linear>());
    else if (input.at("color").at("name") == "periodic") return run(input, fractal, alg, input.at("color").get<Periodic>());
    else if (input.at("color").at("name") == "solid") return run(input, fractal, alg, input.at("color").get<Solid>());
    else throw std::runtime_error("color name '" + std::string(input.at("color").at("name")) + "' not found.");
}

template<class F>
Image run(const json& input, const F& fractal) {
    using namespace Algorithm; std::string name = input.at("algorithm").at("name");
    if (name == "density") return run(input, fractal, input.at("algorithm").get<Density>());
    else if (name == "escape") return run(input, fractal, input.at("algorithm").get<Escape>());
    else if (name == "orbitrap") return run(input, fractal, input.at("algorithm").get<Orbitrap>());
    else throw std::runtime_error("Fractal name '" + name + "' not found.");
}

Image run(const json& input) {
    using namespace Fractal;
    if (input.at("fractal").at("name") == "buffalo") return run(input, Buffalo{});
    else if (input.at("fractal").at("name") == "burningship") return run(input, BurningShip{});
    else if (input.at("fractal").at("name") == "julia") return run(input, Julia{ { 0, 1 } });
    else if (input.at("fractal").at("name") == "mandelbrot") return run(input, Mandelbrot{});
    else if (input.at("fractal").at("name") == "manowar") return run(input, Manowar{});
    else if (input.at("fractal").at("name") == "phoenix") return run(input, Phoenix{ { 0, 0 } });
    else throw std::runtime_error("Fractal name '" + std::string(input.at("fractal").at("name")) + "' not found.");
}

int main(int argc, char** argv) {
    argparse::ArgumentParser program("Fractoid", "1.0", argparse::default_arguments::none);

    program.add_argument("-f").help("Fractoid input file.").default_value(std::string("fractal.json"));
    program.add_argument("-h").help("Display this help message and exit.").default_value(false).implicit_value(true);
    program.add_argument("-q").help("Suppress all standard output.").default_value(false).implicit_value(true);

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << std::endl << std::endl << program; return EXIT_FAILURE;
    }

    if (program.get<bool>("-h")) {
        std::cout << program.help().str(); return EXIT_SUCCESS;
    }

    if (!std::filesystem::exists(program.get<std::string>("-f"))) {
        throw std::runtime_error(std::string("File '") + program.get<std::string>("-f") + std::string("' does not exist."));
    }

    json input = patch(json::parse(std::ifstream(program.get<std::string>("-f"))));

    Timer timer(true); Image image = run(input);
    if (!program.get<bool>("-q")) std::cout << "Fractal generation took " << timer.elapsed() << " ms." << std::endl;
    image.save(input.value("output", OUTPUT));
    if (!program.get<bool>("-q")) std::cout << "Fractal export took " << timer.elapsed() << " ms." << std::endl;
}
