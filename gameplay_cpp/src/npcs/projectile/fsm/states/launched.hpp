#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <npcs/projectile/projectile.h>

#include <debugdraw3d/api.h>
#include <configparams.hpp>
#include <godot_cpp/classes/timer.hpp>

namespace fsm::projectile {

class Launched : public BaseState {
	TYPE(Launched)

	ConfigParam::Npcs::Projectile param;

private:
	godot::Timer* m_timer = nullptr;

public:
	VState getType() const override {
		return TLaunched();
	}

	VState enter(Context& context) override {
		m_timer = memnew(godot::Timer);
		ASSERTNN(m_timer)

		context.owner->add_child(m_timer);
		m_timer->connect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
		m_timer->start(param.lifetime());

		return {};
	}

	VState exit(Context& context) override {
		ASSERTNN(m_timer)
		m_timer->stop();
		m_timer->disconnect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
		context.owner->remove_child(m_timer);
		m_timer->queue_free();

		return {};
	}

	VState handleExternalAction(Context& context, const VExternalEvent& action) override {
		if (std::holds_alternative<EventAttack>(action)) {
			utils::death(context);
		}
		else if (std::holds_alternative<EventParry>(action)) {
			const auto& a = std::get<EventParry>(action);
			context.owner->set_linear_velocity(godot::Vector3(0, a.params.lift, 0));

			// this gets called during integrateForces, which does not support setting position directly
			context.owner->set_deferred("position", a.instigatorPosition + (a.params.direction * a.params.length));
			return TParried{};
		}
		return {};
	}

	VState physicsProcess(Context& context, float delta) {
		return {};
	}
};

} //namespace fsm::projectile