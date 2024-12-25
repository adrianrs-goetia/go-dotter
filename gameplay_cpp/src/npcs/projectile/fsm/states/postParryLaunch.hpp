#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <npcs/projectile/projectile.h>

#include <godot_cpp/classes/timer.hpp>

#include <configparams.hpp>

namespace fsm::projectile {

class PostParryLaunch : public BaseState {
	TYPE(PostParryLaunch)

	ConfigParam::Npcs::Projectile param;

private:
	Timestamp m_enterTime;

public:
	VState getType() const override {
		return TPostParryLaunched();
	}

	VState enter(Context& context) override {
		m_enterTime.setTimestamp();
		return {};
	}

	VState exit(Context& context) override {
		return {};
	}

	VState handleExternalAction(Context& context, const VExternalEvent& action) override {
		return {};
	}

	VState physicsProcess(Context& context, float delta) {
		if (m_enterTime.timestampWithinTimeframe(param.postParryIntagibleTime())) {
			return TLaunched();
		}
		return {};
	}
};

} //namespace fsm::projectile