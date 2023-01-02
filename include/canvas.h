#pragma once

#include "buffer.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>

class Canvas {
public:
    Canvas(int width, int height);
    void render(const Shader& shader) const;
    void resize(int width, int height);

private:
    glm::mat4 scale;
    Buffer buffer;
};
