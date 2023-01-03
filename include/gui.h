#pragma once

#include "algorithm.h"
#include "color.h"
#include "lib/imgui/imgui_impl_opengl3.h"
#include "lib/imgui/imgui_impl_glfw.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Shader;
class Gui;

struct GLFWPointer {
    int width, height, major = 4, minor = 2;
    std::string title = "Fractoid";
    bool mleft = 0, mright = 0;
    glm::vec2 mouse{};
    struct Camera {
        glm::mat4 proj;
    } camera{};
    struct Settings {
        Color::Periodic<float> periodic;
        Algorithm::Escape<float> escape;
        std::complex<double> center;
        double zoom;
    } settings;
    Shader* shader;
    Gui* gui;
};

struct Gui {
    Gui(GLFWwindow* window); ~Gui();
    void render();
    bool fps = 0, options = 0;
    GLFWwindow* window;
};
