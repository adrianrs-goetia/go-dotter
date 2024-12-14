#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <npcs/projectile/projectile.h>

#include <godot_cpp/classes/timer.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "npcs", "projectile"

namespace fsm::projectile {

class PostParryLaunch : public BaseState {
	Timestamp m_enterTime;

public:
	TState getType() const override {
		return TPostParryLaunched();
	}

	TState enter(Context& context) override {
		m_enterTime.setTimestamp();
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState handleExternalAction(Context& context, const ExternalAction& action) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) {
		if (m_enterTime.timestampWithinTimeframe(GETPARAM_F("postParryIntagibleTime"))) {
			return TLaunched();
		}
		return {};
	}
};

#undef CONFIG_PREFIX

} //namespace fsm::projectile