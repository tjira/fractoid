<h1 align="center">Fractoid</h1>

<p align="center">
    <a href="https://github.com/tjira/fractoid/pulse">
        <img src="https://img.shields.io/github/last-commit/tjira/fractoid?logo=github&logoColor=white&style=for-the-badge"/>
    </a>
    <a href="https://github.com/tjira/fractoid/blob/master/LICENSE.md">
        <img src="https://img.shields.io/github/license/tjira/fractoid?logo=gitbook&logoColor=white&style=for-the-badge"/>
    </a>
    <a href="https://github.com/tjira/fractoid/stargazers">
        <img src="https://img.shields.io/github/stars/tjira/fractoid?logo=apachespark&logoColor=white&style=for-the-badge"/>
    </a>
    <a href="https://github.com/tjira/fractoid">
        <img src="https://img.shields.io/github/languages/code-size/tjira/fractoid?logo=databricks&logoColor=white&style=for-the-badge"/>
    </a>
    <br>
    <a href="https://github.com/tjira/fractoid/releases/latest">
        <img src="https://img.shields.io/github/v/release/tjira/fractoid?display_name=tag&logo=sharp&logoColor=white&style=for-the-badge"/>
    </a>
    <a href="https://github.com/tjira/fractoid/releases/latest">
        <img src="https://img.shields.io/github/downloads/tjira/fractoid/total?logo=markdown&logoColor=white&style=for-the-badge"/>
    </a>
</p>

<p align="center">
Fractoid is a simple software to generate beautiful and customizable fractal images. The program has a GUI that uses the GPU or CLI with input in JSON format that runs on CPU.
</p>

## ✨ Features

Below are all the fractals and algorithms that are implemented in fractoid.

### Fractal Algorithms

* Buffalo Fractal
* Burning Ship
* Julia Set
* Mandelbrot Set
* Manowar Fractal
* Phoenix Fractal

### Generation Algorithm

* Escape Time Based Algorithm
* Orbitrap Algorithm

### Coloring Algorithms

* Linear Coloring
* Solid Coloring
* Periodic Coloring

## 🛠️ Compilation

The compilation is meant for linux users. You can compile it similarly on windows, but I recommend downloading the latest release binary. Before compilation, please install some basic X libraries.

```bash
sudo apt install libxcursor-dev libxi-dev libxinerama-dev libxrandr-dev
```

After that, you can compile fractoid simply by the following command.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release . && cmake --build build
```

The binary will be compiled in the *bin* folder.

## 🖥️ Examples

If you simply execute the binary you will launch the GUI. The navigation there is intuitively done by mouse. Options are available after pressing the `F1` key. To use the CLI, you need to create an input file. Let's create an input file called `input.json` with the following contents.

```json
{
    "fractal" : {
        "name" : "mandelbrot"
    },
    "algorithm" : {
        "name" : "escape"
    },
    "color" : {
        "name" : "periodic"
    },
    "center" : [ -0.75, 0 ],
    "zoom" : 1.1
}
```

This example generates Mandelbrot set with escape time based generation algorithm and periodic coloring. To generate a *png* image simply run

```bash
fractoid -f input.json
```

File `fractal.png` will be created in the current directory.

## ⭐ Credits

* [argparse](https://github.com/p-ranav/argparse) - Argument Parser for Modern C++.
* [eigen](https://gitlab.com/libeigen/eigen) - Eigen is a C++ template library for linear algebra.
* [glad](https://github.com/Dav1dde/glad) - Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
* [glfw](https://github.com/glfw/glfw) - A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input .
* [glm](https://github.com/g-truc/glm) - OpenGL Mathematics.
* [imgui](https://github.com/ocornut/imgui) - Bloat-free Graphical User Interface for C++ with minimal dependencies.
* [imguifiledialog](https://github.com/aiekick/ImGuiFileDialog) - File dialog for Dear ImGui.
* [implot](https://github.com/epezent/implot) - Immediate mode plotting for ImGui.
* [libint](https://github.com/evaleev/libint) - High-performance library for computing Gaussian integrals in quantum mechanics.
* [libxc](https://gitlab.com/libxc/libxc) - Library of exchange-correlation functionals for density-functional theory.
* [stb](https://github.com/nothings/stb) - Single-file public domain libraries for C/C++.
