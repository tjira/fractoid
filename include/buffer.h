#pragma once

#include "../lib/glad/gl.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec2 position;
};

class Buffer {
public:
    Buffer(const std::vector<Vertex>& data);
    ~Buffer() { glDeleteVertexArrays(1, &vao), glDeleteBuffers(1, &vbo); };
    void bind() const { glBindVertexArray(vao); };
    size_t getSize() const { return data.size(); };

private:
    std::vector<Vertex> data;
    unsigned int vao, vbo;
};
