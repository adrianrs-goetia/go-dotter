#pragma once

#include "godotincludes.h"
#include <random>

// Directions
static const godot::Vector3 g_up(0, 1, 0);
static const godot::Vector3 g_forward(0, 0, 1);
static const godot::Vector3 g_right(1, 0, 0);

// Math constants
constexpr float PI = 3.14159f;
constexpr float PI_HALF = 3.14159f / 2.f;
constexpr float PI_TWO = 3.14159f * 2.f;

inline float canonicalRandomNumber(float min, float max) {
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<> dis(min, max);
	return dis(rng);
}