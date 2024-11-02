#pragma once

#include <core/core.h>

class ParryInstigatorComponent;
class ParryTargetComponent;

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

	ParryInstance(const ParryInstigatorComponent& instigator, const ParryTargetComponent& target);

	godot::Vector3 getNewTargetVelocity() const;
};