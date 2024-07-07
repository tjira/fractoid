#pragma once

#include "algorithm.h"
#include "color.h"
#include "fractal.h"
#include "image.h"

#include <random>

struct Options {
    std::array<uchar, 3> background;
    std::array<int, 2> resolution;
};

template <class F, class A, class C>
class Painter {
public:
    Painter(const F& fractral, const A& alg, const C& col, const Options& options);
    Image paint(std::complex<double> center, double zoom) const;

private:
    void density(Image& image, std::complex<double> center, double zoom) const;
    void escape(Image& image, std::complex<double> center, double zoom) const;
    void orbitrap(Image& image, std::complex<double> center, double zoom) const;
    template <typename T> std::tuple<std::complex<double>, T> loop(std::complex<double> p) const;

private:
    Options options;
    A alg; C col;
    F fractal;
};

template <class F, class A, class C>
Painter<F, A, C>::Painter(const F& fractal, const A& alg, const C& col, const Options& options) : options(options), alg(alg), col(col), fractal(fractal) {}

template <class F, class A, class C>
Image Painter<F, A, C>::paint(std::complex<double> center, double zoom) const {
    // create the image object with provided resolution and background
    Image image(options.resolution.at(0), options.resolution.at(1), options.background);

    // decide what algorithm to use and paint it
    if constexpr (std::is_same<A, Algorithm::Density>()) density(image, center, zoom);
    if constexpr (std::is_same<A, Algorithm::Escape>()) escape(image, center, zoom);
    if constexpr (std::is_same<A, Algorithm::Orbitrap>()) orbitrap(image, center, zoom);
    
    return image; // return the image
}

template <class F, class A, class C> template <typename T>
std::tuple<std::complex<double>, T> Painter<F, A, C>::loop(std::complex<double> p) const {
    // define the used variables
    std::complex<double> z, zp, tmp;

    // perform the calculation
    if constexpr (std::is_same<A, Algorithm::Escape>()) {
        double value = alg.iterations;
        if constexpr (std::is_same<F, Fractal::BurningShip>()) {BURNINGSHIP(ESCAPE)}
        if constexpr (std::is_same<F, Fractal::Buffalo>()) {BUFFALO(ESCAPE)}
        if constexpr (std::is_same<F, Fractal::Julia>()) {JULIA(ESCAPE)}
        if constexpr (std::is_same<F, Fractal::Mandelbrot>()) {MANDELBROT(ESCAPE)}
        if constexpr (std::is_same<F, Fractal::Manowar>()) {MANOWAR(ESCAPE)}
        if constexpr (std::is_same<F, Fractal::Phoenix>()) {PHOENIX(ESCAPE)}
        return {z, value};
    } else {
        std::vector<std::complex<double>> orbit;
        if constexpr (std::is_same<F, Fractal::Buffalo>()) {BUFFALO(ORBIT)}
        if constexpr (std::is_same<F, Fractal::BurningShip>()) {BURNINGSHIP(ORBIT)}
        if constexpr (std::is_same<F, Fractal::Julia>()) {JULIA(ORBIT)}
        if constexpr (std::is_same<F, Fractal::Mandelbrot>()) {MANDELBROT(ORBIT)}
        if constexpr (std::is_same<F, Fractal::Manowar>()) {MANOWAR(ORBIT)}
        if constexpr (std::is_same<F, Fractal::Phoenix>()) {PHOENIX(ORBIT)}
        return {z, orbit};
    }
}

template<class F, class A, class C>
void Painter<F, A, C>::density(Image &image, std::complex<double> center, double zoom) const {using namespace Fractal;
    // create the distribution and the mersenne twister
    std::uniform_real_distribution<double> uniform(-3.5, 3.5); std::mt19937 gen(alg.seed);
    
    // create the container for the random positions                                                                                          
    std::vector<std::complex<double>> positions(alg.samples);

    // generate the random positions
    for (int i = 0; i < alg.samples; i++) {positions.at(i) = {uniform(gen), uniform(gen)};}

    // create the data for the image
    std::vector<int> data(image.getWidth() * image.getHeight(), 0);

    // loop over the random positions
    for (std::complex<double> p : positions) {

        // obtain the position orbit
        auto [z, orbit] = loop<std::vector<std::complex<double>>>(p);

        // loop over the orbit
        if (std::norm(z) > alg.bailout * alg.bailout) for (std::complex<double> o : orbit) {

            // transform the complex number to pixel coordinates
            int i = std::round(((o.imag() + center.imag()) * image.getHeight() * zoom + 1.5 * image.getHeight()) / 3.0 - 0.5);
            int j = std::round(((o.real() - center.real()) * image.getHeight() * zoom + 1.5 * image.getWidth())  / 3.0 - 0.5);

            // check if the pixel is in the image
            if (i < 0 || j < 0 || i >= image.getHeight() || j >= image.getWidth()) continue;

            // increment the pixel color
            data.at(i * image.getWidth() + j)++;
        }
    }

    // get the maximum value of the image for normalization
    int max = *std::max_element(data.begin(), data.end());

    #if defined(_OPENMP)
    #pragma omp parallel for collapse(2)
    #endif
    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            if (double value = (double)data.at(i * image.getWidth() + j) / max; value) image(i, j) = col.get(value);
        }
    }
}

template <class F, class A, class C>
void Painter<F, A, C>::escape(Image& image, std::complex<double> center, double zoom) const {using namespace Fractal;
    #if defined(_OPENMP)
    #pragma omp parallel for collapse(2)
    #endif
    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {

            // get the real and imaginary part of the complex number located at the pixel
            double im = -center.imag() + (3.0 * (i + 0.5) - 1.5 * image.getHeight()) / zoom / image.getHeight();
            double re =  center.real() + (3.0 * (j + 0.5) - 1.5 * image.getWidth())  / zoom / image.getHeight();

            // perform the algorithm
            auto [z, value] = loop<double>({ re, im });

            // assign the pixel color
            if (value < alg.iterations) {
                if (alg.smooth) {value -= std::log2(0.5 * std::log(std::norm(z)));} image(i, j) = col.get(value / alg.iterations);
            }
        }
    }
}

template <class F, class A, class C>
void Painter<F, A, C>::orbitrap(Image& image, std::complex<double> center, double zoom) const {using namespace Fractal;
    #if defined(_OPENMP)
    #pragma omp parallel for collapse(2)
    #endif
    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {

            // get the real and imaginary part of the complex number located at the pixel
            double im = -center.imag() + (3.0 * (i + 0.5) - 1.5 * image.getHeight()) / zoom / image.getHeight();
            double re =  center.real() + (3.0 * (j + 0.5) - 1.5 * image.getWidth())  / zoom / image.getHeight();

            // perform the algorithm
            auto [z, orbit] = loop<std::vector<std::complex<double>>>({re, im});

            // assign the pixel color
            if (double value = 1e10, distance = 0; !alg.fill || std::norm(z) > alg.bailout * alg.bailout) {

                // get the minimal distance from the trap
                for (std::complex<double> p : orbit) {if (distance = DISTANCE(p); distance < value) value = distance;}

                // edit the value for better image
                if constexpr (std::is_same<C, Color::Periodic>()) value = 0.03 * std::log(value);
                if constexpr (std::is_same<C, Color::Linear>())   value = 1 / (1 + 5 * value);
                
                // assogn the color
                image(i, j) = col.get(value);
            }
        }
    }
}
