#include "include/callback.h"
#include "include/canvas.h"
#include "include/defaults.h"
#include "include/image.h"
#include "include/painter.h"
#include "lib/argparse/argparse.hpp"
#include <filesystem>

json patch(json input) {
    std::string algorithm = input.at("algorithm").at("name"), color = input.at("color").at("name");
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
    else if (input.at("fractal").at("name") == "julia") return run(input, Julia{ { 0, 1 } });
    else if (input.at("fractal").at("name") == "mandelbrot") return run(input, Mandelbrot{});
    else if (input.at("fractal").at("name") == "manowar") return run(input, Manowar{});
    else if (input.at("fractal").at("name") == "phoenix") return run(input, Phoenix{ { 0, 0 } });
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

void gui(const argparse::ArgumentParser& program) {
    // Create GLFW variable struct and a window pointer
    GLFWPointer pointer; GLFWwindow* window;

    // Initialize GLFW and throw error if failed
    if(!glfwInit()) {
        throw std::runtime_error("Error during GLFW initialization.");
    }

    // Pass OpenGL version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, pointer.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, pointer.minor);

    // Create the window
    if (window = glfwCreateWindow(pointer.width, pointer.height, pointer.title.c_str(), nullptr, nullptr); !window) {
        throw std::runtime_error("Error during window creation.");
    }

    // Initialize GLAD
    if (glfwMakeContextCurrent(window); !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Error during GLAD initialization.");
    }

    // Enable some options
    glfwSetWindowUserPointer(window, &pointer);
    glfwSwapInterval(1);

    // Set event callbacks
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, positionCallback);
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    // Initialize camera projection matrix
    pointer.camera.proj = glm::ortho(0.0f, (float)pointer.width, (float)pointer.height, 0.0f, -1.0f, 1.0f);
    
    {
        // Create canvas, shader and GUI
        new(&pointer.shader) Shader(vertex, generateFractalShader("mandelbrot", "escape", "periodic"));
        Canvas canvas(pointer.width, pointer.height);
        Gui gui(window);

        // Save GUI to the GLFW variable
        pointer.gui = &gui;

        // Create periodic coloring struct
        pointer.settings.escape = Defaults::escape.get<Algorithm::Escape>();
        pointer.settings.orbitrap = Defaults::orbitrap.get<Algorithm::Orbitrap>();
        pointer.settings.linear = Defaults::linear.get<Color::Linear>();
        pointer.settings.periodic = Defaults::periodic.get<Color::Periodic>();
        pointer.settings.solid = Defaults::solid.get<Color::Solid>();
        pointer.settings.center = { -0.75, 0 };
        pointer.settings.zoom = 1.2;

        // Enter the render loop
        while (!glfwWindowShouldClose(window)) {

            // Clear the color and depth buffer
            glClear(GL_COLOR_BUFFER_BIT);

            // Set the canvas to the correct height
            canvas.resize(pointer.width, pointer.height);

            // Set variables to shader
            pointer.shader.set("u_proj", pointer.camera.proj);
            pointer.shader.set("zoom", pointer.settings.zoom);
            pointer.shader.set("re", pointer.settings.center.real());
            pointer.shader.set("im", pointer.settings.center.imag());
            pointer.shader.set("width", pointer.width);
            pointer.shader.set("height", pointer.height);

            // Set periodic coloring variables to shader
            if (pointer.state.at(1) == "escape") {
                pointer.shader.set("alg.iters", pointer.settings.escape.iterations);
                pointer.shader.set("alg.bail", pointer.settings.escape.bailout);
                pointer.shader.set("alg.smoothing", pointer.settings.escape.smooth);
            } else if (pointer.state.at(1) == "orbitrap") {
                pointer.shader.set("alg.iters", pointer.settings.orbitrap.iterations);
                pointer.shader.set("alg.bail", pointer.settings.orbitrap.bailout);
                pointer.shader.set("alg.trap", pointer.settings.orbitrap.trap);
                pointer.shader.set("alg.fill", pointer.settings.orbitrap.fill);
            }
            if (pointer.state.at(2) == "linear") {
                pointer.shader.set("col.from", pointer.settings.linear.from);
                pointer.shader.set("col.to", pointer.settings.linear.to);
            } else if(pointer.state.at(2) == "periodic") {
                pointer.shader.set("col.r1", pointer.settings.periodic.amplitude.at(0));
                pointer.shader.set("col.g1", pointer.settings.periodic.amplitude.at(1));
                pointer.shader.set("col.b1", pointer.settings.periodic.amplitude.at(2));
                pointer.shader.set("col.r2", pointer.settings.periodic.phase.at(0));
                pointer.shader.set("col.g2", pointer.settings.periodic.phase.at(1));
                pointer.shader.set("col.b2", pointer.settings.periodic.phase.at(2));
            } else if (pointer.state.at(2) == "solid") {
                pointer.shader.set("col.rgb", pointer.settings.solid.rgb);
            }
            if (pointer.state.at(0) == "julia") {
                pointer.shader.set("fractal.cRe", pointer.settings.julia.c.real());
                pointer.shader.set("fractal.cIm", pointer.settings.julia.c.imag());
            } else if(pointer.state.at(0) == "phoenix") {
                pointer.shader.set("fractal.cRe", pointer.settings.phoenix.c.real());
                pointer.shader.set("fractal.cIm", pointer.settings.phoenix.c.imag());
            }

            // Zoom and move
            if ((pointer.mleft || pointer.mright) && !ImGui::GetIO().WantCaptureMouse) {
                glm::vec2 vector = 0.0002f * (pointer.mouse - glm::vec2(pointer.width / 2, pointer.height / 2));
                pointer.settings.center += std::complex<double>(vector.x, vector.y) / pointer.settings.zoom;
                pointer.settings.zoom /= pointer.mright ? 1.02 : 1;
                pointer.settings.zoom *= pointer.mleft ? 1.02 : 1;
            }

            // Render canvas and GUI
            canvas.render(pointer.shader);
            gui.render();

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    // Clean up GLFW
    glfwTerminate();
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

    program.is_used("-f") ? cli(program) : gui(program);
}
