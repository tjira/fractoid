#include "include/defaults.h"
#include "include/canvas.h"
#include "include/gui.h"
#include "include/shader.h"
#include "include/uniform.h"
#include "lib/argparse/argparse.hpp"
#include <iostream>
#include <fstream>

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
uniform float re, im, bail, zoom;
out vec4 o_color;

float mandelbrot(float pRe, float pIm) {
    float zRe = 0; float zIm = 0;
    for(int n = 0; n < iters; n++){
        float temp = zRe * zRe - zIm * zIm + pRe;
        zIm = 2 * zRe * zIm + pIm; zRe = temp;
        if(zRe * zRe + zIm * zIm > bail * bail) {
            return n - log2(0.5 * log(zRe * zRe + zIm * zIm));
        }
    }
    return iters;
}

vec4 colorPeriodic(float value) {
    float r = (sin(periodic.r1 * value / iters + periodic.r2) + 1.0f) / 2.0f;
    float g = (sin(periodic.g1 * value / iters + periodic.g2) + 1.0f) / 2.0f;
    float b = (sin(periodic.b1 * value / iters + periodic.b2) + 1.0f) / 2.0f;
    return vec4(r, g, b, 1.0f);
}

void main() {
    float pIm = -im + (3.0f * (gl_FragCoord.y + 0.5f) - 1.5f * height) / zoom / height;
    float pRe = re + (3.0f * (gl_FragCoord.x + 0.5f) - 1.5f * width) / zoom / height;
    float value = mandelbrot(pRe, pIm);
    if (value < iters) {
        o_color = colorPeriodic(value);
    } else {
        o_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
})";

void keyCallback(GLFWwindow* window, int key, int, int action, int mods) {
    if (GLFWPointer* pointer = (GLFWPointer*)glfwGetWindowUserPointer(window); action == GLFW_PRESS) {
        if (key == GLFW_KEY_F1) pointer->gui->options = !pointer->gui->options; 
        if (key == GLFW_KEY_F12) pointer->gui->fps = !pointer->gui->fps; 
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
    program.add_argument("-f").help("Fractoid input file.").default_value(std::string("fractal.json"));

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
    
    // Parse the input file
    json input = Defaults::patch(json::parse(std::ifstream(program.get<std::string>("-f"))));

    // Create GLFW variable struct and a window pointer
    GLFWPointer pointer { .width = std::vector<int>{RESOLUTION}[0], .height = std::vector<int>{RESOLUTION}[1] };
    GLFWwindow* window;

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
        Canvas canvas(pointer.width, pointer.height);
        Gui gui(window); pointer.gui = &gui;
        Shader shader(vertex, fragment);

        // Create periodic coloring struct
        Uniform a(input.at("color").at("amplitude"), input.at("color").at("seed").at(0));
        Uniform p(input.at("color").at("phase"), input.at("color").at("seed").at(1));
        pointer.settings.periodic = Color::Periodic<float> {
            .r1 = (float)a.get(), .g1 = (float)a.get(), .b1 = (float)a.get(),
            .r2 = (float)p.get(), .g2 = (float)p.get(), .b2 = (float)p.get()
        };
        pointer.settings.escape = input.at("algorithm").get<Algorithm::Escape<float>>();
        pointer.settings.center = input.at("center").get<std::complex<double>>();
        pointer.settings.zoom = input.at("zoom");

        // Enter the render loop
        while (!glfwWindowShouldClose(window)) {

            // Clear the color and depth buffer
            glClear(GL_COLOR_BUFFER_BIT);

            // Set the canvas to the correct height
            canvas.resize(pointer.width, pointer.height);

            // Set variables to shader
            shader.set("u_proj", pointer.camera.proj);
            shader.set("iters", pointer.settings.escape.iterations);
            shader.set("bail", pointer.settings.escape.bailout);
            shader.set("zoom", pointer.settings.zoom);
            shader.set("re", pointer.settings.center.real());
            shader.set("im", pointer.settings.center.imag());
            shader.set("width", pointer.width);
            shader.set("height", pointer.height);

            // Set periodic coloring variables to shader
            shader.set("periodic.r1", pointer.settings.periodic.r1);
            shader.set("periodic.g1", pointer.settings.periodic.g1);
            shader.set("periodic.b1", pointer.settings.periodic.b1);
            shader.set("periodic.r2", pointer.settings.periodic.r2);
            shader.set("periodic.g2", pointer.settings.periodic.g2);
            shader.set("periodic.b2", pointer.settings.periodic.b2);

            // Zoom and move
            if ((pointer.mleft || pointer.mright) && !ImGui::GetIO().WantCaptureMouse) {
                glm::vec2 vector = 0.0002f * (pointer.mouse - glm::vec2(pointer.width / 2, pointer.height / 2));
                pointer.settings.center += std::complex<double>(vector.x, vector.y) / pointer.settings.zoom;
                pointer.settings.zoom /= pointer.mright ? 1.02 : 1;
                pointer.settings.zoom *= pointer.mleft ? 1.02 : 1;
            }

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
