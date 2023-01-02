#include "../include/buffer.h"

Buffer::Buffer(const std::vector<Vertex>& data) : data(data) {
    glGenVertexArrays(1, &vao), glGenBuffers(1, &vbo), glBindBuffer(GL_ARRAY_BUFFER, vbo), glBindVertexArray(vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), data.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
}
