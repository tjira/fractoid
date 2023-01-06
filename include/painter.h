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
Painter<F, A, C>::Painter(const F& fractal, const A& alg, const C& col, const Options& options) : options(options) {
    this->alg = alg, this->col = col, this->fractal = fractal;
    if constexpr (std::is_same<C, Color::Periodic>()) {
        std::uniform_real_distribution<double> a(col.amplitude.at(0), col.amplitude.at(1));
        std::uniform_real_distribution<double> p(col.phase.at(0), col.phase.at(1));
        std::mt19937 ag(col.seed.at(0)), pg(col.seed.at(1));
        this->col.r1 = a(ag), this->col.g1 = a(ag), this->col.b1 = a(ag);
        this->col.r2 = p(pg), this->col.g2 = p(pg), this->col.b2 = p(pg);
    }
}

template <class F, class A, class C>
Image Painter<F, A, C>::paint(std::complex<double> center, double zoom) const {
    Image image(options.resolution.at(0), options.resolution.at(1), options.background);
    if constexpr (std::is_same<A, Algorithm::Density>()) density(image, center, zoom);
    if constexpr (std::is_same<A, Algorithm::Escape>()) escape(image, center, zoom);
    if constexpr (std::is_same<A, Algorithm::Orbitrap>()) orbitrap(image, center, zoom);
    return image;
}

template <class F, class A, class C> template <typename T>
std::tuple<std::complex<double>, T> Painter<F, A, C>::loop(std::complex<double> p) const {
    std::complex<double> z, zp, tmp;
    if constexpr (std::is_same<A, Algorithm::Escape>()) {
        double value = alg.iterations;
        if constexpr (std::is_same<F, Fractal::BurningShip>()) { BURNINGSHIP(ESCAPE) }
        if constexpr (std::is_same<F, Fractal::Buffalo>()) { BUFFALO(ESCAPE) }
        if constexpr (std::is_same<F, Fractal::Julia>()) { JULIA(ESCAPE) }
        if constexpr (std::is_same<F, Fractal::Mandelbrot>()) { MANDELBROT(ESCAPE) }
        if constexpr (std::is_same<F, Fractal::Manowar>()) { MANOWAR(ESCAPE) }
        if constexpr (std::is_same<F, Fractal::Phoenix>()) { PHOENIX(ESCAPE) }
        return { z, value };
    } else {
        std::vector<std::complex<double>> orbit;
        if constexpr (std::is_same<F, Fractal::Buffalo>()) { BUFFALO(ORBIT) }
        if constexpr (std::is_same<F, Fractal::BurningShip>()) { BURNINGSHIP(ORBIT) }
        if constexpr (std::is_same<F, Fractal::Julia>()) { JULIA(ORBIT) }
        if constexpr (std::is_same<F, Fractal::Mandelbrot>()) { MANDELBROT(ORBIT) }
        if constexpr (std::is_same<F, Fractal::Manowar>()) { MANOWAR(ORBIT) }
        if constexpr (std::is_same<F, Fractal::Phoenix>()) { PHOENIX(ORBIT) }
        return { z, orbit };
    }
}

template<class F, class A, class C>
void Painter<F, A, C>::density(Image &image, std::complex<double> center, double zoom) const {using namespace Fractal;
    std::uniform_real_distribution<double> uniform(-3.5, 3.5); std::mt19937 gen(alg.seed);
    std::vector<std::complex<double>> positions(alg.samples);
    for (int i = 0; i < alg.samples; i++) positions.at(i) = { uniform(gen), uniform(gen) };
    std::vector<int> data(image.getWidth() * image.getHeight(), 0);
    for (std::complex<double> p : positions) {
        auto [z, orbit] = loop<std::vector<std::complex<double>>>(p);
        if (std::norm(z) > alg.bailout * alg.bailout) for (std::complex<double> o : orbit) {
            int i = std::round(((o.imag() + center.imag()) * image.getHeight() * zoom + 1.5 * image.getHeight()) / 3.0 - 0.5);
            int j = std::round(((o.real() - center.real()) * image.getHeight() * zoom + 1.5 * image.getWidth()) / 3.0 - 0.5);
            if (i < 0 || j < 0 || i >= image.getHeight() || j >= image.getWidth()) continue;
            data.at(i * image.getWidth() + j)++;
        }
    }
    int max = *std::max_element(data.begin(), data.end());
    #pragma omp parallel for default(none) shared(image, data, max)
    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            if (double value = (double)data.at(i * image.getWidth() + j) / max; value) {
                image(i, j) = col.get(value);
            }
        }
    }
}

template <class F, class A, class C>
void Painter<F, A, C>::escape(Image& image, std::complex<double> center, double zoom) const {using namespace Fractal;
    #pragma omp parallel for default(none) shared(image, center, zoom)
    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            double im = -center.imag() + (3.0 * (i + 0.5) - 1.5 * image.getHeight()) / zoom / image.getHeight();
            double re = center.real() + (3.0 * (j + 0.5) - 1.5 * image.getWidth()) / zoom / image.getHeight();
            auto [z, value] = loop<double>({ re, im });
            if (value < alg.iterations) {
                if (alg.smooth) value -= std::log2(0.5 * std::log(std::norm(z)));
                image(i, j) = col.get(value / alg.iterations);
            }
        }
    }
}

template <class F, class A, class C>
void Painter<F, A, C>::orbitrap(Image& image, std::complex<double> center, double zoom) const {using namespace Fractal;
    #pragma omp parallel for default(none) shared(image, center, zoom)
    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            double im = -center.imag() + (3.0 * (i + 0.5) - 1.5 * image.getHeight()) / zoom / image.getHeight();
            double re = center.real() + (3.0 * (j + 0.5) - 1.5 * image.getWidth()) / zoom / image.getHeight();
            auto [z, orbit] = loop<std::vector<std::complex<double>>>({ re, im });
            if (double value = 1e10, distance = 0; !alg.fill || std::norm(z) > alg.bailout * alg.bailout) {
                for (std::complex<double> p : orbit) {
                    if (distance = DISTANCE(p); distance < value) value = distance;
                }
                if constexpr (std::is_same<C, Color::Periodic>()) value = 0.03 * std::log(value);
                if constexpr (std::is_same<C, Color::Linear>()) value = 1 / (1 + 5 * value);
                image(i, j) = col.get(value);
            }
        }
    }
}
