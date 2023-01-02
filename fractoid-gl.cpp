#include "include/color.h"
#include "include/defaults.h"
#include "include/canvas.h"
#include "include/gui.h"
#include "include/shader.h"
#include "include/uniform.h"
#include "lib/argparse/argparse.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

std::string vertex = R"(
#version 420 core
layout (location = 0) in vec3 i_position;
uniform mat4 u_model, u_proj;
void main() {
    gl_Position = u_proj * u_model * vec4(i_position, 1.0);
})";

std::string fragment = R"(
#version 420 core

struct Periodic { float r1, g1, b1, r2, g2, b2; };

uniform Periodic periodic;

uniform int width, height, iters;
uniform float bail, zoom, x, y;
out vec4 o_color;

float mandelbrot(float re, float im) {
    float zRe = 0; float zIm = 0;
    for(int n = 0; n < iters; n++){
        float temp = zRe * zRe - zIm * zIm + re;
        zIm = 2 * zRe * zIm + im; zRe = temp;
        if(zRe * zRe + zIm * zIm > bail * bail) {
            return n - log2(0.5 * log(zRe * zRe + zIm * zIm));
        }
    }
    return iters;
}

void main() {
    float im = -y + (3.0 * (gl_FragCoord.y + 0.5) - 1.5 * height) / zoom / height;
    float re = x + (3.0 * (gl_FragCoord.x + 0.5) - 1.5 * width) / zoom / height;
    float value = mandelbrot(re, im);
    if (value < iters) {
        float r = (sin(periodic.r1 * value / iters + periodic.r2) + 1) / 2;
        float g = (sin(periodic.g1 * value / iters + periodic.g2) + 1) / 2;
        float b = (sin(periodic.b1 * value / iters + periodic.b2) + 1) / 2;
        o_color = vec4(r, g, b, 1.0f);
    } else {
        o_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
})";

struct GLFWPointer {
    int width = WIDTH, height = HEIGHT, major = 4, minor = 2;
    std::string title = "OpenGL Fractal Explorer";
    glm::vec2 mouse{};
    struct Camera {
        glm::mat4 proj;
    } camera{};
};

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
    glfwSetWindowSizeCallback(window, resizeCallback);

    // Initialize camera projection matrix
    pointer.camera.proj = glm::ortho(0.0f, (float)pointer.width, (float)pointer.height, 0.0f, -1.0f, 1.0f);
    
    {
        // Create canvas, shader and GUI
        Shader shader(vertex, fragment);
        Canvas canvas(WIDTH, HEIGHT);
        Gui gui(window);

        // Create periodic coloring struct
        Uniform a(Defaults::periodic.at("amplitude"), Defaults::periodic.at("seed").at(0));
        Uniform p(Defaults::periodic.at("phase"), Defaults::periodic.at("seed").at(1));
        Color::Periodic periodic {
            .r1 = a.get(), .g1 = a.get(), .b1 = a.get(),
            .r2 = p.get(), .g2 = p.get(), .b2 = p.get()
        };

        // Enter the render loop
        while (!glfwWindowShouldClose(window)) {

            // Clear the color and depth buffer
            glClear(GL_COLOR_BUFFER_BIT);

            // Set the canvas to the correct height
            canvas.resize(pointer.width, pointer.height);

            // Set variables to shader
            shader.set("u_proj", pointer.camera.proj);
            shader.set("iters", 80);
            shader.set("bail", 10.0f);
            shader.set("zoom", 1.2f);
            shader.set("x", -0.75f);
            shader.set("y", 0.0f);
            shader.set("width", pointer.width);
            shader.set("height", pointer.height);

            // Set periodic coloring variables to shader
            shader.set("periodic.r1", periodic.r1);
            shader.set("periodic.g1", periodic.g1);
            shader.set("periodic.b1", periodic.b1);
            shader.set("periodic.r2", periodic.r2);
            shader.set("periodic.g2", periodic.g2);
            shader.set("periodic.b2", periodic.b2);

            // Render canvas and GUI
            canvas.render(shader);
            gui.render();

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    // Clean up GLFW
    glfwTerminate();
}
