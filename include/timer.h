#pragma once

#include <chrono>

class Timer {
public:
    Timer(bool autoreset = false);
    unsigned int elapsed();
    void reset();

private:
    std::chrono::high_resolution_clock::time_point start;
    bool autoreset;
};
