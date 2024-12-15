#pragma once

#include <core/core.hpp>

#include <components/parryInstigatorComponent.h>
#include <components/parryTargetComponent.hpp>

#include <variant>

struct ParryInstance {
	const godot::Vector3 instigatorPosition;
	const godot::Vector3 targetPosition;
	const ParryInstigatorComponent::ActivateParams instigatorParams;

	ParryInstance(const ParryInstigatorComponent& instigator, const ParryTargetComponent& target,
		const ParryInstigatorComponent::ActivateParams params)
		: instigatorPosition(instigator.getPosition())
		, targetPosition(target.getPosition())
		, instigatorParams(params) {}

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

struct ParryFreezeInstance {
	float time;
};

struct PlayerParryJump {};

/**
 * Trickery to get compiler to be able to use a variant of these types with just a forward declaration of the Wrapper
 */
struct WrapperParryInstances {
	using T = std::variant<ParryInstance, ParryFreezeInstance, PlayerParryJump>;
	T obj;

	WrapperParryInstances(T t)
		: obj(t) {}
};
