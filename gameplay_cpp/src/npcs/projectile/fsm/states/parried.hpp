#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"

#include <npcs/projectile/projectile.h>

namespace fsm::projectile {

class Parried : public BaseState {
	TYPE(Parried)

private:
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
		return std::visit(
			overloaded{
				[&](EventAttack action) -> VState
				{
					const auto dir = action.getDirection();
					context.owner->set_linear_velocity(dir * action.attackStrength);
					return TPostParryLaunched();
				},
				[&](EventParryJump) -> VState
				{
					utils::death(context);
					return std::monostate();
				},
				[](auto a) -> VState
				{
					ASSERT(false, "Projectile state # ", Name(), " # got unexpected event: ", a.Name())
					return std::monostate();
				},
			},
			action);
	}

	VState physicsProcess(Context& context, float delta) override {
		if (context.owner->isOnGround()) {
			utils::death(context);
		}
		return {};
	}
};

} //namespace fsm::projectile