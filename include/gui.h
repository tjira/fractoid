#pragma once

#include "lib/imgui/imgui_impl_opengl3.h"
#include "lib/imgui/imgui_impl_glfw.h"

class Gui {
public:
    Gui(GLFWwindow* window); ~Gui();
    void render();

private:
    GLFWwindow* window;
};
