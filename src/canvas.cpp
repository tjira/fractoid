#include "../include/canvas.h"

Canvas::Canvas(int width, int height) : buffer({
    {{ -1.0f,  1.0f }},
    {{ -1.0f, -1.0f }},
    {{  1.0f, -1.0f }},
    {{  1.0f, -1.0f }},
    {{  1.0f,  1.0f }},
    {{ -1.0f,  1.0f }}
}) {
    scale = glm::scale(glm::mat4(1.0f), { width, height, 1.0f });
}

void Canvas::render(const Shader& shader) const {
    shader.use(), shader.set<glm::mat4>("u_model", scale), buffer.bind();
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)buffer.getSize());
}

void Canvas::resize(int width, int height) {
    scale = glm::scale(glm::mat4(1.0f), { width, height, 1.0f });
}
