#pragma once

#include <core/core.hpp>

struct EventAttack {
	const godot::Transform3D instigatorTransform;
	const godot::Transform3D targetTransform;
	const float attackStrength;
	const float launchAngle;

	godot::Vector3 getDirection() const {
		using namespace godot;
		const Vector3 instigatorPos = instigatorTransform.get_origin();
		const Vector3 targetPos = targetTransform.get_origin();
		return Vector3(targetPos - instigatorPos).normalized();
	}

	godot::Vector3 getDirection2D() const {
		const godot::Vector3 dir = targetTransform.get_origin() - instigatorTransform.get_origin();
		return godot::Vector3(dir.x, 0, dir.z).normalized();
	}

	godot::Vector3 getDirectionAverage() const {
		return godot::Vector3(getDirection() + getDirection2D()).normalized();
	}

	godot::Vector3 getDir2DRotated() const {
		const auto dir = getDirection2D();
		const auto right = g_up.cross(dir);
		return dir.rotated(right, godot::Math::deg_to_rad(launchAngle));
	}
};
