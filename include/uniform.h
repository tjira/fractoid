#pragma once

#include <array>
#include <chrono>
#include <random>

class Uniform {
public:
	Uniform(const std::array<double, 2>& range, long seed = 0);
	double get() { return dist(generator); };
	long getSeed() const { return seed; };

private:
	std::uniform_real_distribution<double> dist;
	std::mt19937 generator;
	long seed;
};

