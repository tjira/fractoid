#include "include/canvas.h"
#include "include/gui.h"
#include "include/shader.h"
#include "lib/argparse/argparse.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define WIDTH 1024
#define HEIGHT 576

std::string vertex = R"(
#version 420 core
layout (location = 0) in vec3 i_position;
uniform mat4 u_model, u_proj;
void main() {
    gl_Position = u_proj * u_model * vec4(i_position, 1.0);
})";

std::string fragment = R"(
#version 420 core
uniform int width, height, iters;
uniform float bail, zoom, x, y;
out vec4 o_color;

float mandelbrot(float re, float im) {
    float zRe = 0; float zIm = 0;
    for(int n = 0; n < iters; n++){
        float temp = zRe * zRe - zIm * zIm + re;
        zIm = 2 * zRe * zIm + im; zRe = temp;
        if(zRe * zRe + zIm * zIm > bail * bail) {
            return n - log2(0.5 * log(sqrt(zRe * zRe + zIm * zIm)));
        }
    }
    return iters;
}

void main() {
    float im = -y + (3.0 * (gl_FragCoord.y + 0.5) - 1.5 * height) / zoom / height;
    float re = x + (3.0 * (gl_FragCoord.x + 0.5) - 1.5 * width) / zoom / height;
    float value = mandelbrot(re, im);
    if (value < iters) {
        o_color = vec4(value / 255.0f, value / 255.0f, value / 255.0f, 1.0f);
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

        // Enter the render loop
        while (!glfwWindowShouldClose(window)) {

            // Clear the color and depth buffer
            glClear(GL_COLOR_BUFFER_BIT);

            // Set the canvas to the correct height
            canvas.resize(pointer.width, pointer.height);

            // Set variables to shader
            shader.set("u_proj", pointer.camera.proj);
            shader.set("iters", 80);
            shader.set("bail", 5.0f);
            shader.set("zoom", 1.2f);
            shader.set("x", -0.75f);
            shader.set("y", 0.0f);
            shader.set("width", pointer.width);
            shader.set("height", pointer.height);

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
