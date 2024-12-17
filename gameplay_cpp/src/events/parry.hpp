#pragma once

#include <core/core.hpp>

#include <variant>

struct EventParry {
	struct Params {
		const godot::Vector3 direction;
		const float length;
		const float lift;
	};

	const godot::Vector3 instigatorPosition;
	const godot::Vector3 targetPosition;
	const Params params;
	
	godot::Vector3 getVectorToTarget() const {
		return godot::Vector3(targetPosition - instigatorPosition);
	}

	godot::Vector3 getDirectionToTarget3D() const {
		return getVectorToTarget().normalized();
	}

	godot::Vector3 getDirectionToTarget2D() const {
		const auto dir3D = getVectorToTarget();
		return godot::Vector3(dir3D.x, 0, dir3D.z).normalized();
	}
};

struct EventParryFreeze {
	float time;
};

struct EventParryJump {};

using TEventParry = std::variant<EventParry, EventParryFreeze, EventParryJump>;
