#pragma once

#include "algorithm.h"
#include "color.h"
#include "fractal.h"
#include "shader.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <ImGuiFileDialog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

struct Gui;

struct GLFWPointer {
    std::vector<std::string> state = { "mandelbrot", "escape", "periodic" };
    int width = 1280, height = 720, major = 4, minor = 2;
    bool mleft = 0, mright = 0, fullscreen = 0;
    std::string title = "Fractoid";
    glm::vec2 mouse{};
    struct Camera {
        glm::mat4 proj;
    } camera{};
    struct Settings {
        Color::Linear linear;
        Color::Periodic periodic;
        Color::Solid solid;
        Fractal::Julia julia;
        Fractal::Phoenix phoenix;
        Algorithm::Escape escape;
        Algorithm::Orbitrap orbitrap;
        std::complex<double> center;
        double zoom;
    } settings;
    Shader shader;
    Gui* gui;
};

struct Gui {
    Gui(GLFWwindow* window); ~Gui();
    void render();
    bool fps = 0, options = 0;
    GLFWwindow* window;
};
