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

namespace godot::Math {

inline godot::Vector3 move_toward(godot::Vector3 p_from, godot::Vector3 p_to, float p_delta) {
	return godot::Vector3(godot::Math::move_toward(p_from.x, p_to.x, p_delta),
		godot::Math::move_toward(p_from.y, p_to.y, p_delta),
		godot::Math::move_toward(p_from.z, p_to.z, p_delta));
}

inline godot::Vector3 move_toward(godot::Vector3 p_from, godot::Vector2 p_to, float p_delta) {
	return godot::Vector3(godot::Math::move_toward(p_from.x, p_to.x, p_delta),
		p_from.y,
		godot::Math::move_toward(p_from.z, p_to.y, p_delta));
}

} //namespace godot::Math
