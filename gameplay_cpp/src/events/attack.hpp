#pragma once

#include <core/core.hpp>

struct EventAttack {
	const godot::Transform3D instigatorTransform;
	const godot::Transform3D targetTransform;
	const float attackStrength;

	godot::Vector3 getDirection() const {
		using namespace godot;
		const Vector3 instigatorPos = instigatorTransform.get_origin();
		const Vector3 targetPos = targetTransform.get_origin();
		return Vector3(targetPos - instigatorPos).normalized();
	}
};
