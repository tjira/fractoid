#include "../include/shader.h"
#include <include/algorithm.h>

std::string generateFractalShader(std::string fractal, std::string algorithm, std::string color) {
    std::string shader = "#version 420 core\nfloat norm(float re,float im){return re*re+im*im;}";
    shader += "struct Fract{float cRe,cIm;};struct Alg{int iters,trap;float bail;bool fill,smoothing;};struct Col{vec3 from,to,rgb;float r1,g1,b1,r2,g2,b2;};";
    shader += "uniform Fract fractal;uniform Alg alg;uniform Col col;uniform float re,im,zoom;uniform int width,height;out vec4 o_color;";
    std::function<std::string(std::string)> main = [](std::string normalization) { return R"(
        void main() {
            float pIm = im - (3.0f * (gl_FragCoord.y + 0.5f) - 1.5f * height) / zoom / height;
            float pRe = re + (3.0f * (gl_FragCoord.x + 0.5f) - 1.5f * width) / zoom / height;
            float value = function(pRe, pIm);
            o_color = value < alg.iters ? colorize()"+normalization+R"() : vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
    )";};
    std::string fractalInit, fractalCode, normalization = "value/alg.iters"; std::function<std::string(std::string, std::string)> function;
    if (fractal == "buffalo") fractalInit = Fractal::Buffalo::code.init, fractalCode = Fractal::Buffalo::code.code;
    else if (fractal == "burningship") fractalInit = Fractal::BurningShip::code.init, fractalCode = Fractal::BurningShip::code.code;
    else if (fractal == "julia") fractalInit = Fractal::Julia::code.init, fractalCode = Fractal::Julia::code.code;
    else if (fractal == "mandelbrot") fractalInit = Fractal::Mandelbrot::code.init, fractalCode = Fractal::Mandelbrot::code.code;
    else if (fractal == "manowar") fractalInit = Fractal::Manowar::code.init, fractalCode = Fractal::Manowar::code.code;
    else if (fractal == "phoenix") fractalInit = Fractal::Phoenix::code.init, fractalCode = Fractal::Phoenix::code.code;
    else throw std::runtime_error("Fractal name does not exist.");
    if (algorithm == "escape") shader += Algorithm::Escape::code(fractalInit, fractalCode);
    else if (algorithm == "orbitrap") shader += Algorithm::Orbitrap::code(fractalInit, fractalCode);
    else throw std::runtime_error("Algorithm name does not exist.");
    if (color == "linear") shader += Color::Linear::code;
    else if (color == "periodic") shader += Color::Periodic::code;
    else if (color == "solid") shader += Color::Solid::code;
    else throw std::runtime_error("Color name does not exist.");
    if (algorithm == "orbitrap" && color == "linear") normalization = "1 / (1 + 5 * value)";
    if (algorithm == "orbitrap" && color == "periodic") normalization = "0.03 * log(value)";
    return shader + main(normalization);
}

Shader::Shader(const std::string& vertex, const std::string& fragment) : id(glCreateProgram()) {
    const char *fsCode = fragment.c_str(), *vsCode = vertex.c_str();
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsCode, nullptr);
    glShaderSource(fs, 1, &fsCode, nullptr);
    glCompileShader(vs), glCompileShader(fs);
    errorCheck(vs, "vertex"), errorCheck(fs, "fragment");
    glAttachShader(id, vs), glAttachShader(id, fs);
    glLinkProgram(id), glValidateProgram(id);
    glDetachShader(id, vs), glDetachShader(id, fs);
    glDeleteShader(vs), glDeleteShader(fs), use();
}

void Shader::errorCheck(unsigned int shader, const std::string& title) const {
    int success; glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(int length = 0; success == 0) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        throw std::runtime_error(title + ": " + std::string(log.begin(), log.end()));
    }
}
