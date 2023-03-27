#pragma once

#include "gui.h"

void keyCallback(GLFWwindow* window, int key, int, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void positionCallback(GLFWwindow* window, double x, double y);
void resizeCallback(GLFWwindow* window, int width, int height);
