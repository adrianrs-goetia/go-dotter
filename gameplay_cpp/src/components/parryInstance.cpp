#include "parryInstance.h"
#include "parryinstigatorcomponent.h"
#include "parrytargetcomponent.h"

constexpr float massLimit = 0.001f;

ParryInstance::ParryInstance(const ParryInstigatorComponent& instigator, const ParryTargetComponent& target)
	: instigatorDesiredDirection(instigator.getDesiredDirection())
	, targetPosition(target.getPosition())
	, targetVelocity(target.getVelocity())
	, targetDesiredDirection(target.getDesiredDirection()) {
	if (instigator.getMass() < massLimit && target.getMass() < massLimit) {
		LOG(WARN, "Neither instigator or target has any mass for Parry Instance. Setting weight to 0.5f")
		weight = 0.5f;
	}
	else if (instigator.getMass() < massLimit) { weight = 1.f; }
	else if (target.getMass() < massLimit) { weight = 0.f; }
	else {
		weight = 1.f /
				(1 + (100 * Math::exp(-(target.getMass() / instigator.getMass()) * 9.f))); // Magic values attached to
																						   // logistic sigmoid / soft
																						   // step To ensure we get a
																						   // value in range of 0-1
	}
}

godot::Vector3 ParryInstance::getNewTargetVelocity() const {
	const godot::Vector3 newDir =
			godot::Vector3(targetDesiredDirection * weight + ((1.f - weight) * instigatorDesiredDirection))
					.normalized();
	return newDir * targetVelocity.length();
}
