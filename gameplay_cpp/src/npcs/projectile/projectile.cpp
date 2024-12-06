#include "projectile.h"

#include <components/parryTargetComponent.h>

#include <godot_cpp/classes/scene_tree.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "npcs", "projectile"

using namespace godot;

void Projectile::_bind_methods() {}

void Projectile::_enter_tree() {

	RETURN_IF_EDITOR(void())
	//

	m_timer = memnew(Timer);
	m_parryTargetComp = getChildOfNode<ParryTargetComponent>(this);

	ASSERT_NOTNULL(m_timer)
	ASSERT_NOTNULL(m_parryTargetComp)

	add_child(m_timer);
	m_timer->connect("timeout", callable_mp(this, &Projectile::onTimeout));
	m_timer->start(GETPARAM_D("lifetime"));

}

void Projectile::_physics_process(double delta) {}

void Projectile::onTimeout() {
	queue_free();
}
