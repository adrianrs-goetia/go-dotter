#include "launched.h"

#include "_utils.h"
#include <npcs/projectile/projectile.h>

#include <godot_cpp/classes/timer.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "npcs", "projectile"

using namespace godot;

namespace fsm::projectile {

TState Launched::enter(Context& context) {
	m_timer = memnew(godot::Timer);
	ASSERT_NOTNULL(m_timer)

	context.owner->add_child(m_timer);
	m_timer->connect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
	m_timer->start(GETPARAM_D("lifetime"));

	return {};
}

TState Launched::exit(Context& context) {
	ASSERT_NOTNULL(m_timer)
	m_timer->stop();
	m_timer->disconnect("timeout", callable_mp(context.owner, &Projectile::onTimeout));
	context.owner->remove_child(m_timer);
	m_timer->queue_free();

	return {};
}

TState Launched::handleExternalAction(Context& context, const ExternalAction& action) {
	if (std::holds_alternative<AttackInstance>(action)) {
		utils::death(context);
	}
	else if (std::holds_alternative<ParryInstance>(action)) {
		context.owner->set_linear_velocity(Vector3(0, 3, 0));
		return TParried{};
	}
	return {};
}

} //namespace fsm::projectile