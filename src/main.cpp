#include <boost/program_options.hpp>
#include "BurningShip.h"
#include "Julia.h"
#include "Mandelbrot.h"
#include "Newton.h"
#include "Phoenix.h"
#include "Manowar.h"


int main(int argc, const char *argv[]) {
    boost::program_options::variables_map arguments;
    boost::program_options::options_description description("Possible options:");
    description.add_options()
            ("center,c", boost::program_options::value<std::complex<double>>()->default_value(std::complex<double>(0, 0)), "Center coordinates.")
            ("escape,e", boost::program_options::value<int>()->default_value(5), "Escape radius.")
            ("fractal,f", boost::program_options::value<std::string>()->default_value("phoenix"), "Fractal name.")
            ("help,h", "Produce help message.")
            ("iterations,i", boost::program_options::value<int>()->default_value(100), "Maximum number of iterations.")
            ("resolution,r", boost::program_options::value<std::vector<int>>()->multitoken()->default_value(std::vector<int>{1024, 768}, "1024, 768"), "Resolution of the image.")
            ("seed,s", boost::program_options::value<uint64>(&cv::theRNG().state)->default_value(4294967295), "RNG seed used for coloring.")
            ("zoom,z", boost::program_options::value<double>()->default_value(1), "Zoom factor.")
            ("complex", boost::program_options::value<std::complex<double>>()->default_value(std::complex<double>(0, 0)), "Additional complex parameter.")
            ("no-smooth", boost::program_options::bool_switch(), "Disable smoothing.");
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), arguments);
    boost::program_options::notify(arguments);
    if (arguments.count("help")) {
        std::cout << description;
        return 0;
    }
    if (arguments["fractal"].as<std::string>() == "burningship") {
        BurningShip fractal(arguments["iterations"].as<int>(), arguments["escape"].as<int>(), !arguments["no-smooth"].as<bool>());
        fractal.show(arguments["resolution"].as<std::vector<int>>(), arguments["center"].as<std::complex<double>>(), arguments["zoom"].as<double>());
    } else if (arguments["fractal"].as<std::string>() == "julia") {
        Julia fractal(arguments["iterations"].as<int>(), arguments["escape"].as<int>(), !arguments["no-smooth"].as<bool>(), arguments["complex"].as<std::complex<double>>());
        fractal.show(arguments["resolution"].as<std::vector<int>>(), arguments["center"].as<std::complex<double>>(), arguments["zoom"].as<double>());
    } else if (arguments["fractal"].as<std::string>() == "mandelbrot") {
        Mandelbrot fractal(arguments["iterations"].as<int>(), arguments["escape"].as<int>(), !arguments["no-smooth"].as<bool>());
        fractal.show(arguments["resolution"].as<std::vector<int>>(), arguments["center"].as<std::complex<double>>(), arguments["zoom"].as<double>());
    } else if (arguments["fractal"].as<std::string>() == "manowar") {
        Manowar fractal(arguments["iterations"].as<int>(), arguments["escape"].as<int>(), !arguments["no-smooth"].as<bool>());
        fractal.show(arguments["resolution"].as<std::vector<int>>(), arguments["center"].as<std::complex<double>>(), arguments["zoom"].as<double>());
    } else if (arguments["fractal"].as<std::string>() == "newton") {
        Newton fractal(arguments["iterations"].as<int>());
        fractal.show(arguments["resolution"].as<std::vector<int>>(), arguments["center"].as<std::complex<double>>(), arguments["zoom"].as<double>());
    } else if (arguments["fractal"].as<std::string>() == "phoenix") {
        Phoenix fractal(arguments["iterations"].as<int>(), arguments["escape"].as<int>(), !arguments["no-smooth"].as<bool>(), arguments["complex"].as<std::complex<double>>());
        fractal.show(arguments["resolution"].as<std::vector<int>>(), arguments["center"].as<std::complex<double>>(), arguments["zoom"].as<double>());
    }
    return 0;
}
