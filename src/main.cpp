#include "defaults.h"
#include "image.h"
#include "painter.h"

json patch(json input) {
    // get the names of the fractal, algorithm and color
    std::string fractal = input.at("fractal").at("name"), algorithm = input.at("algorithm").at("name"), color = input.at("color").at("name");

    // merging the input with the default values
    if (fractal == "julia") Defaults::julia.merge_patch(input.at("fractal")), input.at("fractal") = Defaults::julia;
    if (fractal == "phoenix") Defaults::phoenix.merge_patch(input.at("fractal")), input.at("fractal") = Defaults::phoenix;
    if (algorithm == "density") Defaults::density.merge_patch(input.at("algorithm")), input.at("algorithm") = Defaults::density;
    if (algorithm == "escape") Defaults::escape.merge_patch(input.at("algorithm")), input.at("algorithm") = Defaults::escape;
    if (algorithm == "orbitrap") Defaults::orbitrap.merge_patch(input.at("algorithm")), input.at("algorithm") = Defaults::orbitrap;
    if (color == "linear") Defaults::linear.merge_patch(input.at("color")), input.at("color") = Defaults::linear;
    if (color == "periodic") Defaults::periodic.merge_patch(input.at("color")), input.at("color") = Defaults::periodic;
    if (color == "solid") Defaults::solid.merge_patch(input.at("color")), input.at("color") = Defaults::solid;

    return input; // return the patchaed input
}

template<class F, class A, class C>
Image run(const json& input, const F& fractal, const A& alg, const C& col) {
    // define the default background color and the default image size
    std::array<uchar, 3> background = input.value("background", std::array<uchar, 3>{0, 0, 0});
    std::array<int, 2> resolution = input.value("resolution", std::array<int, 2>{1920, 1080});

    // create the painter class
    Painter painter(fractal, alg, col, { background, resolution });

    // return the painted image
    return painter.paint(input.at("center"), input.at("zoom"));
}

template<class F, class A>
Image run(const json& input, const F& fractal, const A& alg) {
    // use the color namespace and extract the color name
    using namespace Color; std::string name = input.at("color").at("name");

    // choose the color
    if (name == "linear") return run(input, fractal, alg, input.at("color").get<Linear>());
    else if (name == "periodic") return run(input, fractal, alg, input.at("color").get<Periodic>());
    else if (name == "solid") return run(input, fractal, alg, input.at("color").get<Solid>());
    
    // throw error if the color is not found
    else throw std::runtime_error("COLOR NAME '" + name + "' NOT FOUND");
}

template<class F>
Image run(const json& input, const F& fractal) {
    // use the algorithm namespace and extract the algorithm name
    using namespace Algorithm; std::string name = input.at("algorithm").at("name");

    // choose the algorithm
    if (name == "density") return run(input, fractal, input.at("algorithm").get<Density>());
    else if (name == "escape") return run(input, fractal, input.at("algorithm").get<Escape>());
    else if (name == "orbitrap") return run(input, fractal, input.at("algorithm").get<Orbitrap>());

    // throw error if the algorithm is not found
    else throw std::runtime_error("ALGORITHM '" + name + "' NOT FOUND");
}

Image run(const json& input) {
    // use the fractal namespace and extract the fractal name
    using namespace Fractal; std::string name = input.at("fractal").at("name");

    // choose the fractal
    if (name == "buffalo") return run(input, Buffalo{});
    else if (name == "burningship") return run(input, BurningShip{});
    else if (name == "julia") return run(input, input.at("fractal").get<Julia>());
    else if (name == "mandelbrot") return run(input, Mandelbrot{});
    else if (name == "manowar") return run(input, Manowar{});
    else if (name == "phoenix") return run(input, input.at("fractal").get<Phoenix>());
    
    // throw error if the fractal is not found
    else throw std::runtime_error("FRACTAL NAME '" + name + "' NOT FOUND");
}

void cli(const argparse::ArgumentParser& program) {
    // check if the file exists
    if (!std::filesystem::exists(program.get<std::string>("-f"))) {
        throw std::runtime_error("FILE '" + program.get<std::string>("-f") + "' DOES NOT EXIST");
    }

    // parse and patch the json file
    json input = patch(json::parse(std::ifstream(program.get<std::string>("-f"))));

    // get the program start time
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock().now();

    // paint the image
    Image image = run(input);

    // save the time
    long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - start).count();
    
    // save the image
    image.save(input.value("output", "fractal.png"));
    
    // save the time
    long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - start).count();

    // print the elapsed time if the quiet option was not provided
    if (!program.get<bool>("-q")) std::cout << "Fractal generation took " << t1 << " ms." << std::endl;
    if (!program.get<bool>("-q")) std::cout << "Fractal export took " << t2 - t1 << " ms." << std::endl;
}

int main(int argc, char** argv) {
    // create the command line parser
    argparse::ArgumentParser program("Fractoid", "1.0", argparse::default_arguments::none);

    // add the command line options
    program.add_argument("-f").help("Fractoid input file.").default_value("fractal.json");
    program.add_argument("-h").help("Display this help message and exit.").default_value(false).implicit_value(true);
    program.add_argument("-q").help("Suppress all standard output.").default_value(false).implicit_value(true);

    // parse the command line arguments
    try {program.parse_args(argc, argv);
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << std::endl << std::endl << program; return EXIT_FAILURE;
    }

    // print the help
    if (program.get<bool>("-h")) {std::cout << program.help().str(); return EXIT_SUCCESS;}

    cli(program); // run the cli function
}
