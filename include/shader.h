#pragma once

#include "../include/algorithm.h"
#include "../include/color.h"
#include "../include/fractal.h"
#include "../lib/GL/gl.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>
#include <string>
#include <stdexcept>
#include <vector>

static std::string vertex = R"(
#version 420 core
layout (location = 0) in vec3 i_position;
uniform mat4 u_model, u_proj;
void main() {
    gl_Position = u_proj * u_model * vec4(i_position, 1.0);
})";
std::string generateFractalShader(std::string fractal, std::string algorithm, std::string color);

class Shader {
public:
    Shader(const std::string& vertex, const std::string& fragment);
    Shader() = default; ~Shader() { glDeleteProgram(id); };
    void use() const { glUseProgram(id); };
    template <typename T> void set(const std::string& name, T value) const;

private:
    void errorCheck(unsigned int shader, const std::string& title) const;
    unsigned int id;
};

template <typename T>
void Shader::set(const std::string& name, T value) const {
    if constexpr (std::is_same<T, bool>()) glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    else if constexpr (std::is_same<T, int>()) glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    else if constexpr (std::is_same<T, double>()) glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    else if constexpr (std::is_same<T, float>()) glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    else if constexpr (std::is_same<T, glm::vec3>()) glUniform3f(glGetUniformLocation(id, name.c_str()), value[0], value[1], value[2]);
    else if constexpr (std::is_same<T, glm::vec4>()) glUniform4f(glGetUniformLocation(id, name.c_str()), value[0], value[1], value[2], value[3]);
    else if constexpr (std::is_same<T, glm::mat4>()) glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
    else if constexpr (std::is_same<T, std::array<int, 3>>()) set(name, glm::vec3(value[0], value[1], value[2]));
    else throw std::runtime_error("Type not known.");
}
