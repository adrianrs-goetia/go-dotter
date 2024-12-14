#pragma once

#include "../typedef.h"
#include "_utils.h"
#include <npcs/projectile/projectile.h>

#include <godot_cpp/classes/timer.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "npcs", "projectile"

namespace fsm::projectile {

class Launched : public BaseState {
	godot::Timer* m_timer = nullptr;

public:
	TState getType() const override {
		return TLaunched();
	}

	TState enter(Context& context) override {
		m_timer = memnew(godot::Timer);
		ASSERT_NOTNULL(m_timer)

		context.owner->add_child(m_timer);
		m_timer->connect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
		m_timer->start(GETPARAM_D("lifetime"));

		return {};
	}

	TState exit(Context& context) override {
		ASSERT_NOTNULL(m_timer)
		m_timer->stop();
		m_timer->disconnect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
		context.owner->remove_child(m_timer);
		m_timer->queue_free();

		return {};
	}

	TState handleExternalAction(Context& context, const ExternalAction& action) override {
		if (std::holds_alternative<AttackInstance>(action)) {
			utils::death(context);
		}
		else if (std::holds_alternative<ParryInstance>(action)) {
			context.owner->set_linear_velocity(godot::Vector3(0, 3, 0));
			return TParried{};
		}
		return {};
	}

	TState physicsProcess(Context& context, float delta) {
		return {};
	}
};

#undef CONFIG_PREFIX

} //namespace fsm::projectile