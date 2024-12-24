#pragma once

#include "godotIncludes.hpp"
#include <random>

// Math constants
constexpr float PI = 3.14159f;
constexpr float PI_HALF = 3.14159f / 2.f;
constexpr float PI_TWO = 3.14159f * 2.f;

inline float canonicalRandomNumber(float min, float max) {
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<> dis(min, max);
	return dis(rng);
}

