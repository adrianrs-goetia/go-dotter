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
	TState getType() const override {
		return TParried();
	}

	TState enter(Context& context) override {
		enteredCollisionLayers = context.owner->get_collision_layer();
		const auto newCollisionLayers = collisionflags::staticWorld | collisionflags::attackTarget;
		context.owner->set_collision_layer(newCollisionLayers);
		return {};
	}

	TState exit(Context& context) override {
		context.owner->set_collision_layer(enteredCollisionLayers);
		return {};
	}

	TState handleExternalAction(Context& context, const ExternalAction& action) override {
		TState ret;
		std::visit(
			overloaded{
				[&](const AttackInstance& action)
				{
					const auto dir = action.getDirection();
					context.owner->set_linear_velocity(dir * action.attackStrength);
					ret = TPostParryLaunched();
				},
				[&](const ParryFreezeInstance& action)
				{
					context.forwardedAction = action;
					ret = TParryFreeze();
				},
				[](const auto&) { ASSERT(false) },
			},
			action);

		return ret;
	}

	TState physicsProcess(Context& context, float delta) override {
		if (context.owner->isOnGround()) {
			utils::death(context);
		}
		return {};
	}
};

#undef CONFIG_PREFIX

} //namespace fsm::projectile