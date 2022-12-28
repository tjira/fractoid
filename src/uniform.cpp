#include "../include/uniform.h"

Uniform::Uniform(const std::array<double, 2>& range, long seed) : dist(range.at(0), range.at(1)), seed(seed) {
    if (seed == 0) this->seed = std::chrono::high_resolution_clock().now().time_since_epoch().count();
	generator = std::mt19937(this->seed);
}
