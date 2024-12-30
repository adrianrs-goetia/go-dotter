#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"

#include <npcs/projectile/projectile.h>

namespace fsm::projectile {

class Parried : public BaseState {
	TYPE(Parried)

private:
	uint32_t enteredCollisionLayers = 0;
	Timestamp entertime;

public:
	VState getType() const override {
		return TParried();
	}

	VState enter(Context& context) override {
		entertime.setTimestamp();
		enteredCollisionLayers = context.owner->get_collision_layer();
		const auto newCollisionLayers = collisionflags::attackTarget;
		context.owner->set_collision_layer(newCollisionLayers);
		return {};
	}

	VState exit(Context& context) override {
		context.owner->set_collision_layer(enteredCollisionLayers);
		return {};
	}

	VState handleExternalEvent(Context& context, VExternalEvent event) override {
		return std::visit(
			overloaded{
				[&](EventAttack event) -> VState
				{
					const auto dir = event.getDir2DRotated();
					context.owner->set_linear_velocity(dir * event.attackStrength);
					// {
					// 	auto pos = context.owner->get_global_position();
					// 	DebugDraw::Line(pos, pos + (event.getDirection() * 3.f), Color(), 1.f);
					// 	DebugDraw::Line(pos, pos + (event.getDirection2D() * 3.f), Color(1,1,1), 1.f);
					// 	DebugDraw::Line(pos, pos + (event.getDirectionAverage() * 3.f), Color(1,0,0), 1.f);
					// 	DebugDraw::Line(pos, pos + (event.getDir2DRotated() * 3.f), Color(0,0,1), 1.f);
					// }
					return TPostParryLaunched{};
				},
				[&](EventParryJump) -> VState
				{
					utils::death(context);
					return {};
				},
				[&](EventPlayerCollision) -> VState { return {}; },
				[&](auto a) -> VState
				{
					ASSERT(false, "Projectile state # ", Name(), " # got unexpected event: ", a.Name())
					return {};
				},
			},
			event);
	}

	VState physicsProcess(Context& context, float delta) override {
		if (!entertime.timestampWithinTimeframe(0.4f) // This anti death timer could be in config
			&& context.owner->isOnGround()) {
			utils::death(context);
		}
		return {};
	}
};

} //namespace fsm::projectile
