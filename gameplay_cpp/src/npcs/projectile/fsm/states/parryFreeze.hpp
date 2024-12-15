#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <components/dataObjects/parryInstance.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "npcs", "projectile"

namespace fsm::projectile {

class ParryFreeze : public BaseState {
	Timestamp m_enterTime;
	float m_freezetime;

public:
	TState getType() const override {
		return TParryFreeze();
	}

	TState enter(Context& context) override {
		m_enterTime.setTimestamp();
		if (context.forwardedAction) {
			std::visit(
				overloaded{
					[&](const ParryFreezeInstance& action) { m_freezetime = action.time; },
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

	TState exit(Context& context) override {
		return {};
	}

	TState handleExternalAction(Context& context, const ExternalAction& action) override {
		return std::visit(
			overloaded{
				[&](PlayerParryJump)
				{
					utils::death(context);
					return std::monostate();
				},
				[](auto) { return std::monostate(); },
			},
			action);
	}

	TState physicsProcess(Context& context, float delta) {
		if (!m_enterTime.timestampWithinTimeframe(m_freezetime)) {
			return TLaunched();
		}
		context.owner->set_linear_velocity(godot::Vector3());
		context.owner->set_angular_velocity(godot::Vector3());
		return {};
	}
};

#undef CONFIG_PREFIX

} //namespace fsm::projectile