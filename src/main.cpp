#include "defaults.h"
#include "image.h"
#include "painter.h"

json patch(json input) {
    std::string fractal = input.at("fractal").at("name"), algorithm = input.at("algorithm").at("name"), color = input.at("color").at("name");
    if (fractal == "julia") Defaults::julia.merge_patch(input.at("fractal")), input.at("fractal") = Defaults::julia;
    if (fractal == "phoenix") Defaults::phoenix.merge_patch(input.at("fractal")), input.at("fractal") = Defaults::phoenix;
    if (algorithm == "density") Defaults::density.merge_patch(input.at("algorithm")), input.at("algorithm") = Defaults::density;
    if (algorithm == "escape") Defaults::escape.merge_patch(input.at("algorithm")), input.at("algorithm") = Defaults::escape;
    if (algorithm == "orbitrap") Defaults::orbitrap.merge_patch(input.at("algorithm")), input.at("algorithm") = Defaults::orbitrap;
    if (color == "linear") Defaults::linear.merge_patch(input.at("color")), input.at("color") = Defaults::linear;
    if (color == "periodic") Defaults::periodic.merge_patch(input.at("color")), input.at("color") = Defaults::periodic;
    if (color == "solid") Defaults::solid.merge_patch(input.at("color")), input.at("color") = Defaults::solid;
    return input;
}

template<class F, class A, class C>
Image run(const json& input, const F& fractal, const A& alg, const C& col) {
    std::array<uchar, 3> background = input.value("background", std::array<uchar, 3>{ 0, 0, 0 });
    std::array<int, 2> resolution = input.value("resolution", std::array<int, 2>{ 1920, 1080 });
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
    else if (input.at("fractal").at("name") == "julia") return run(input, input.at("fractal").get<Julia>());
    else if (input.at("fractal").at("name") == "mandelbrot") return run(input, Mandelbrot{});
    else if (input.at("fractal").at("name") == "manowar") return run(input, Manowar{});
    else if (input.at("fractal").at("name") == "phoenix") return run(input, input.at("fractal").get<Phoenix>());
    else throw std::runtime_error("Fractal name '" + std::string(input.at("fractal").at("name")) + "' not found.");
}

void cli(const argparse::ArgumentParser& program) {
    if (!std::filesystem::exists(program.get<std::string>("-f"))) {
        throw std::runtime_error(std::string("File '") + program.get<std::string>("-f") + std::string("' does not exist."));
    }

    json input = patch(json::parse(std::ifstream(program.get<std::string>("-f"))));

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock().now();
    Image image = run(input);
    long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - start).count();
    image.save(input.value("output", "fractal.png"));
    long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - start).count();

    if (!program.get<bool>("-q")) std::cout << "Fractal generation took " << t1 << " ms." << std::endl;
    if (!program.get<bool>("-q")) std::cout << "Fractal export took " << t2 - t1 << " ms." << std::endl;
}

int main(int argc, char** argv) {
    argparse::ArgumentParser program("Fractoid", "1.0", argparse::default_arguments::none);

    program.add_argument("-f").help("Fractoid input file.").default_value("fractal.json");
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

    cli(program);
}
