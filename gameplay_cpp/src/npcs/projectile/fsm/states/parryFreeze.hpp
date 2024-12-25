#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <events/parry.hpp>

namespace fsm::projectile {

class ParryFreeze : public BaseState {
	TYPE(ParryFreeze)

private:
	Timestamp m_enterTime;
	float m_freezetime;

public:
	VState getType() const override {
		return TParryFreeze();
	}

	VState enter(Context& context) override {
		m_enterTime.setTimestamp();
		if (context.forwardedAction) {
			std::visit(
				overloaded{
					[&](const EventParryFreeze& action) { m_freezetime = action.time; },
					[](const auto&) { ASSERT(false) },
				},
				context.forwardedAction.value());
			context.forwardedAction = std::nullopt;
		}
		else {
			ASSERT(false)
		}

		context.owner->set_collision_mask_value(collisionflags::player, false);
		context.owner->set_collision_layer_value(collisionflags::dynamicWorld, false);
		return {};
	}

	VState exit(Context& context) override {
		return {};
	}

	VState handleExternalAction(Context& context, const VExternalEvent& action) override {
		return std::visit(
			overloaded{
				[&](EventParryJump)
				{
					utils::death(context);
					return std::monostate();
				},
				[](auto) { return std::monostate(); },
			},
			action);
	}

	VState physicsProcess(Context& context, float delta) {
		if (!m_enterTime.timestampWithinTimeframe(m_freezetime)) {
			return TLaunched();
		}
		context.owner->set_linear_velocity(godot::Vector3());
		context.owner->set_angular_velocity(godot::Vector3());
		return {};
	}
};

} //namespace fsm::projectile