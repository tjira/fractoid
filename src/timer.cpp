#include "../include/timer.h"

Timer::Timer(bool autoreset) : start(std::chrono::high_resolution_clock().now()), autoreset(autoreset) {}

unsigned int Timer::elapsed() {
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - start).count();
    if (autoreset) start = std::chrono::high_resolution_clock().now();
    return (unsigned int)elapsed;
}

void Timer::reset() {
    start = std::chrono::high_resolution_clock().now();
}
