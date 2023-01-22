#include "../include/callback.h"

void keyCallback(GLFWwindow* window, int key, int, int action, int mods) {
    if (GLFWPointer* pointer = (GLFWPointer*)glfwGetWindowUserPointer(window); action == GLFW_PRESS) {
        if (mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {
            if (key == GLFW_KEY_S) {
                ImGuiFileDialog::Instance()->OpenDialog("Export Fractoid Input", "Export Fractoid Input", "Fractoid inputs{.json}, All Files{.*}", ".");
            } else if (key == GLFW_KEY_Q) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        } else if (key == GLFW_KEY_F1) pointer->gui->options = !pointer->gui->options; 
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
