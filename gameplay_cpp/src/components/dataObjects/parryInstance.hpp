#pragma once

#include <core/core.h>

#include <components/parryinstigatorcomponent.h>
#include <components/parrytargetcomponent.h>

/**
 * weight of 0 means instigator 100% overrides targets velocity
 * weight of 1 means target ignores the instigators desired direction
 *
 * weight goes towards -1 if target has heavy property.
 * -1: targetDesiredDirection is equal to velocity on XZ plane
 *  1: targetDesiredDirection is equal to mirrored velocity on XZ plane
 * Targets velocity Y is always mirrored when parried
 */

struct ParryInstance {
	const godot::Vector3 instigatorDesiredDirection;
	const godot::Vector3 targetPosition;
	const godot::Vector3 targetVelocity;
	const godot::Vector3 targetDesiredDirection;
	float weight;

	static constexpr float massLimit = 0.001f;

	ParryInstance(const ParryInstigatorComponent& instigator, const ParryTargetComponent& target)
		: instigatorDesiredDirection(instigator.getDesiredDirection())
		, targetPosition(target.getPosition())
		, targetVelocity(target.getVelocity())
		, targetDesiredDirection(target.getDesiredDirection()) {
		if (instigator.getMass() < massLimit && target.getMass() < massLimit) {
			LOG(WARN, "Neither instigator or target has any mass for ParryInstance. Setting weight to 0.5f")
			weight = 0.5f;
		}
		else if (instigator.getMass() < massLimit) { weight = 1.f; }
		else if (target.getMass() < massLimit) { weight = 0.f; }
		else {
			float exp = Math::exp(-(target.getMass() / instigator.getMass()) * 9.f) * 100.f; // Magic values attached to
																							 // logistic sigmoid / soft
																							 // step To ensure we get a
																							 // value in range of 0-1
			weight = 1.f / (1 + exp);
		}
	}

	godot::Vector3 getNewTargetVelocity() const {
		const godot::Vector3 newDir =
				godot::Vector3(targetDesiredDirection * weight + ((1.f - weight) * instigatorDesiredDirection))
						.normalized();
		return newDir * targetVelocity.length();
	}
};