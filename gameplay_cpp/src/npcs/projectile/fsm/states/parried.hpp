#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"

#include <configHandler.h>
#include <npcs/projectile/projectile.h>

#define CONFIG_PREFIX "npcs", "projectile"

namespace fsm::projectile {

class Parried : public BaseState {
	uint32_t enteredCollisionLayers = 0;

public:
	VState getType() const override {
		return TParried();
	}

	VState enter(Context& context) override {
		enteredCollisionLayers = context.owner->get_collision_layer();
		// const auto newCollisionLayers = collisionflags::staticWorld | collisionflags::attackTarget;
		const auto newCollisionLayers = collisionflags::attackTarget;
		context.owner->set_collision_layer(newCollisionLayers);
		return {};
	}

	VState exit(Context& context) override {
		context.owner->set_collision_layer(enteredCollisionLayers);
		return {};
	}

	VState handleExternalAction(Context& context, const VExternalEvent& action) override {
		VState ret;
		std::visit(
			overloaded{
				[&](const AttackInstance& action)
				{
					const auto dir = action.getDirection();
					context.owner->set_linear_velocity(dir * action.attackStrength);
					ret = TPostParryLaunched();
				},
				[&](const EventParryFreeze& action)
				{
					context.forwardedAction = action;
					ret = TParryFreeze();
				},
				[](const auto&) { ASSERT(false) },
			},
			action);

		return ret;
	}

	VState physicsProcess(Context& context, float delta) override {
		if (context.owner->isOnGround()) {
			utils::death(context);
		}
		return {};
	}
};

#undef CONFIG_PREFIX

} //namespace fsm::projectile