#include "include/canvas.h"
#include "include/defaults.h"
#include "include/gui.h"
#include "lib/argparse/argparse.hpp"
#include <random>

void keyCallback(GLFWwindow* window, int key, int, int action, int mods) {
    if (GLFWPointer* pointer = (GLFWPointer*)glfwGetWindowUserPointer(window); action == GLFW_PRESS) {
        if (key == GLFW_KEY_F1) pointer->gui->options = !pointer->gui->options; 
        else if (key == GLFW_KEY_F11) {
            static int xpos0, ypos0, width0, height0; int xpos, ypos, width, height;
            if (pointer->fullscreen = !pointer->fullscreen; pointer->fullscreen) {
                glfwGetWindowSize(window, &width0, &height0); glfwGetWindowPos(window, &xpos0, &ypos0);
                glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, &width, &height);
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, GLFW_DONT_CARE);
            } else {
                glfwSetWindowMonitor(window, nullptr , xpos0, ypos0, width0, height0, GLFW_DONT_CARE);
            }
            glfwSwapInterval(1);
        }
        else if (key == GLFW_KEY_F12) pointer->gui->fps = !pointer->gui->fps; 
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    GLFWPointer* pointer = (GLFWPointer*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_RIGHT) pointer->mright = action == GLFW_PRESS;
    if (button == GLFW_MOUSE_BUTTON_LEFT) pointer->mleft = action == GLFW_PRESS;
}

void positionCallback(GLFWwindow* window, double x, double y) {
    ((GLFWPointer*)glfwGetWindowUserPointer(window))->mouse = { x, y };
}

void resizeCallback(GLFWwindow* window, int width, int height) {
    if (GLFWPointer* pointer = (GLFWPointer*)glfwGetWindowUserPointer(window); width > 0 && height > 0) {
        pointer->camera.proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f );
        pointer->width = width, pointer->height = height; glViewport(0, 0, width, height);
    }
}

int main(int argc, char** argv) {

    // Initialize argument parser
    argparse::ArgumentParser program("OpenGL Fractal Explorer", "1.0", argparse::default_arguments::none);

    // Add argumens to argument parser
    program.add_argument("-h").help("Display this help message and exit.").default_value(false).implicit_value(true);

    // Parse the arguments
    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << std::endl << std::endl << program; return EXIT_FAILURE;
    }

    // Print the help message if the -h flag was passed
    if (program.get<bool>("-h")) {
        std::cout << program.help().str(); return EXIT_SUCCESS;
    }
    
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
        std::uniform_real_distribution<double> a(Defaults::periodic.at("amplitude").at(0), Defaults::periodic.at("amplitude").at(1));
        std::uniform_real_distribution<double> p(Defaults::periodic.at("phase").at(0), Defaults::periodic.at("phase").at(1));
        std::mt19937 ag(Defaults::periodic.at("seed").at(0)), pg(Defaults::periodic.at("seed").at(1));
        pointer.settings.escape = Defaults::escape.get<Algorithm::Escape>();
        pointer.settings.orbitrap = Defaults::orbitrap.get<Algorithm::Orbitrap>();
        pointer.settings.linear = Defaults::linear.get<Color::Linear>();
        pointer.settings.periodic = Color::Periodic {
            .r1 = (float)a(ag), .g1 = (float)a(ag), .b1 = (float)a(ag),
            .r2 = (float)p(pg), .g2 = (float)p(pg), .b2 = (float)p(pg)
        };
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
                pointer.shader.set("col.r1", pointer.settings.periodic.r1);
                pointer.shader.set("col.g1", pointer.settings.periodic.g1);
                pointer.shader.set("col.b1", pointer.settings.periodic.b1);
                pointer.shader.set("col.r2", pointer.settings.periodic.r2);
                pointer.shader.set("col.g2", pointer.settings.periodic.g2);
                pointer.shader.set("col.b2", pointer.settings.periodic.b2);
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
