#include "../include/shader.h"

std::string generateFractalShader(std::string fractal, std::string algorithm, std::string color) {
    std::string uniforms = "uniform Fractal fractal;uniform Algorithm alg;uniform Color col;uniform float re,im,zoom;uniform int width,height;out vec4 o_color;";
    std::string escapeStruct = "struct Algorithm {int iters;float bail;bool smoothing;};";
    std::string orbitrapStruct = "struct Algorithm {int iters, trap;float bail;bool fill;};";
    std::string linearStruct = "struct Color {vec3 from,to;};";
    std::string periodicStruct = "struct Color {float r1,g1,b1,r2,g2,b2;};";
    std::string solidStruct = "struct Color {vec3 rgb;};";

    std::string buffaloInit = "float zRe = 0; float zIm = 0;";
    std::string buffaloCode = "zRe = abs(zRe), zIm = abs(zIm); float temp = zRe * zRe - zIm * zIm - zRe + pRe; zIm = 2 * zRe * zIm -zIm + pIm; zRe = temp;";

    std::string burningShipInit = "float zRe = 0; float zIm = 0;";
    std::string burningShipCode = "zRe = abs(zRe), zIm = abs(zIm); float temp = zRe * zRe - zIm * zIm + pRe; zIm = 2 * zRe * zIm + pIm; zRe = temp;";

    std::string juliaInit = "float zRe = pRe; float zIm = pIm;";
    std::string juliaCode = "float temp = zRe * zRe - zIm * zIm + fractal.cRe; zIm = 2 * zRe * zIm + fractal.cIm; zRe = temp;";

    std::string mandelbrotInit = "float zRe = 0; float zIm = 0;";
    std::string mandelbrotCode = "float temp = zRe * zRe - zIm * zIm + pRe; zIm = 2 * zRe * zIm + pIm; zRe = temp;";

    std::string manowarInit = "float zRe = pRe; float zIm = pIm, zpRe = pRe, zpIm = pIm;";
    std::string manowarCode = "float temp = zRe * zRe - zIm * zIm + zpRe + pRe, tempzRe = zRe, tempzIm = zIm; zIm = 2 * zRe * zIm + zpIm + pIm; zRe = temp, zpRe = tempzRe, zpIm = tempzIm;";
    
    std::string phoenixInit = "float zRe = -pIm; float zIm = pRe, zpRe = fractal.cRe, zpIm = fractal.cIm;";
    std::string phoenixCode = "float temp = zRe * zRe - zIm * zIm - 0.5f * zpRe + 0.5667, tempzRe = zRe, tempzIm = zIm; zIm = 2 * zRe * zIm - 0.5f * zpIm; zRe = temp, zpRe = tempzRe, zpIm = tempzIm;";

    std::string additional = R"(
        float norm(float re, float im) { 
            return re * re + im * im;
        }
    )";
    
    std::function<std::string(std::string, std::string)> escapeFunc = [](std::string fractalInit, std::string fractalCode) { return R"(
        float escape(float pRe, float pIm) {
            )"+fractalInit+R"(
            for(int n = 0; n < alg.iters; n++){
                )"+fractalCode+R"(
                if(norm(zRe, zIm) > alg.bail * alg.bail) {
                    return n - (alg.smoothing ? log2(0.5 * log(norm(zRe, zIm))) : 0);
                }
            }
            return alg.iters;
        }
    )";};
    std::function<std::string(std::string, std::string)> orbitrapFunc = [](std::string fractalInit, std::string fractalCode) { return R"(
        float orbitrap(float pRe, float pIm) {
            )"+fractalInit+R"(float minDistance = alg.iters;
            for(int n = 0; n < alg.iters; n++){
                )"+fractalCode+R"(
                if(zRe * zRe + zIm * zIm > alg.bail * alg.bail) {
                    return minDistance;
                }
                float distance;
                if (alg.trap == 1) distance = norm(zRe, zIm);
                else if (alg.trap == 2) distance = min(abs(zRe), abs(zIm));
                else if (alg.trap == 3) distance = 0.70710678f * min(abs(zRe - zIm), abs(zRe + zIm));
                else if (alg.trap == 4) distance = abs(norm(zRe, zIm) - 1);
                else if (alg.trap == 5) distance = min(norm(zRe, zIm), abs(norm(zRe, zIm) - 1));
                if (distance < minDistance) minDistance = distance;
            }
            return alg.fill ? alg.iters : minDistance;
        }
    )";};
    std::string linearFunc = R"(
        vec4 colorize(float value) {
            float r = (col.from.x + value * (col.to.x - col.from.x)) / 255.0f;
            float g = (col.from.y + value * (col.to.y - col.from.y)) / 255.0f;
            float b = (col.from.z + value * (col.to.z - col.from.z)) / 255.0f;
            return vec4(r, g, b, 1.0f);
        }
    )";
    std::string periodicFunc = R"(
        vec4 colorize(float value) {
            float r = (sin(col.r1 * value + col.r2) + 1.0f) / 2.0f;
            float g = (sin(col.g1 * value + col.g2) + 1.0f) / 2.0f;
            float b = (sin(col.b1 * value + col.b2) + 1.0f) / 2.0f;
            return vec4(r, g, b, 1.0f);
        }
    )";
    std::string solidFunc = R"(
        vec4 colorize(float value) {
            return vec4(col.rgb / 255.0f, 1.0f);
        }
    )";
    std::function<std::string(std::string)> mainEscape = [](std::string normalization) { return R"(
        void main() {
            float pIm = im - (3.0f * (gl_FragCoord.y + 0.5f) - 1.5f * height) / zoom / height;
            float pRe = re + (3.0f * (gl_FragCoord.x + 0.5f) - 1.5f * width) / zoom / height;
            float value = escape(pRe, pIm);
            o_color = value < alg.iters ? colorize()"+normalization+R"() : vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
    )";};
    std::function<std::string(std::string)> mainOrbitrap = [](std::string normalization) { return R"(
        void main() {
            float pIm = im - (3.0f * (gl_FragCoord.y + 0.5f) - 1.5f * height) / zoom / height;
            float pRe = re + (3.0f * (gl_FragCoord.x + 0.5f) - 1.5f * width) / zoom / height;
            float value = orbitrap(pRe, pIm);
            o_color = value < alg.iters ? colorize()"+normalization+R"() : vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
    )";};
    std::string fractalStruct = "struct Fractal{float cRe,cIm;};", fractalInit, fractalCode;
    if (fractal == "buffalo") fractalInit = buffaloInit, fractalCode = buffaloCode;
    else if (fractal == "burningship") fractalInit = burningShipInit, fractalCode = burningShipCode;
    else if (fractal == "julia") fractalInit = juliaInit, fractalCode = juliaCode;
    else if (fractal == "mandelbrot") fractalInit = mandelbrotInit, fractalCode = mandelbrotCode;
    else if (fractal == "manowar") fractalInit = manowarInit, fractalCode = manowarCode;
    else if (fractal == "phoenix") fractalInit = phoenixInit, fractalCode = phoenixCode;
    else throw std::runtime_error("Fractal name does not exist.");
    if (algorithm == "escape") {
        if (color == "linear") {
            return "#version 420 core\n" + fractalStruct + escapeStruct + linearStruct + uniforms + additional + escapeFunc(fractalInit, fractalCode) + linearFunc + mainEscape("value / alg.iters");
        } else if (color == "periodic") {
            return "#version 420 core\n" + fractalStruct + escapeStruct + periodicStruct + uniforms + additional + escapeFunc(fractalInit, fractalCode) + periodicFunc + mainEscape("value / alg.iters");
        } else if (color == "solid") {
            return "#version 420 core\n" + fractalStruct + escapeStruct + solidStruct + uniforms + additional + escapeFunc(fractalInit, fractalCode) + solidFunc + mainEscape("value");
        } else throw std::runtime_error("Color name does not exist.");
    } else if (algorithm == "orbitrap") {
        if (color == "linear") {
            return "#version 420 core\n" + fractalStruct + orbitrapStruct + linearStruct + uniforms + additional + orbitrapFunc(fractalInit, fractalCode) + linearFunc + mainOrbitrap("1 / (1 + 5 * value)");
        } else if (color == "periodic") {
            return "#version 420 core\n" + fractalStruct + orbitrapStruct + periodicStruct + uniforms + additional + orbitrapFunc(fractalInit, fractalCode) + periodicFunc + mainOrbitrap("0.03 * log(value)");
        } else if (color == "solid") {
            return "#version 420 core\n" + fractalStruct + orbitrapStruct + solidStruct + uniforms + additional + orbitrapFunc(fractalInit, fractalCode) + solidFunc + mainOrbitrap("value");
        } else throw std::runtime_error("Color name does not exist.");
    } else throw std::runtime_error("Algorithm name does not exist.");
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
