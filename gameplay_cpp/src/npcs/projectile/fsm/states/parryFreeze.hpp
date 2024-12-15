#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <components/dataObjects/parryInstance.hpp>
// #include <npcs/projectile/projectile.h>

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
					[](const auto&) { ASSERT(false) },
					[&](const ParryFreezeInstance& action) { m_freezetime = action.time; },
				},
				context.forwardedAction.value());
			context.forwardedAction = std::nullopt;
		}
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState handleExternalAction(Context& context, const ExternalAction& action) override {
		return {};
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