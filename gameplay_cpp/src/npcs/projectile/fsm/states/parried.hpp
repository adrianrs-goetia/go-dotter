#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"

#include <configHandler.h>
#include <npcs/projectile/projectile.h>

#define CONFIG_PREFIX "npcs", "projectile"

namespace fsm::projectile {

class Parried : public BaseState {
public:
	TState getType() const override {
		return TParried();
	}

	TState enter(Context& context) override {
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState handleExternalAction(Context& context, const ExternalAction& action) override {
		if (std::holds_alternative<AttackInstance>(action)) {
			auto& attackInstance = std::get<AttackInstance>(action);
			const auto dir = attackInstance.getDirection();
			context.owner->set_linear_velocity(dir * attackInstance.attackStrength);
			return TPostParryLaunched{};
		}
		return {};
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