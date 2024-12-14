#include "projectile.h"

#include "fsm/fsm.h"

#include <components/attackTargetComponent.h>
#include <components/parryTargetComponent.h>

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "npcs", "projectile"

using namespace godot;

void Projectile::_bind_methods() {
	METHOD_PROPERTY_PACKEDSCENE_IMPL(Projectile, DeathParticles)
}

void Projectile::_enter_tree() {
	RETURN_IF_EDITOR(void())
	//

	// Required for PhysicsDirectBodyState3D to register contacts to be used within _integrate_forces
	set_contact_monitor(true);
	set_max_contacts_reported(3);

	m_fsm = new fsm::projectile::Fsm(fsm::projectile::Context{ this });
	ASSERT_NOTNULL(m_fsm)
	m_fsm->init<fsm::projectile::TLaunched>();

	m_parryTargetComp = getComponentOfNode<ParryTargetComponent>(this);
	m_attackTargetComp = memnew(AttackTargetComponent);
	add_child(m_attackTargetComp);

	ASSERT_NOTNULL(m_parryTargetComp)
	ASSERT_NOTNULL(m_attackTargetComp)

	m_parryTargetComp->_callback = [this](const auto& instance) { this->m_fsm->handleExternalAction(instance); };
	m_attackTargetComp->_callback = [this](const auto& instance) { this->m_fsm->handleExternalAction(instance); };

	add_to_group(godotgroups::projectile);
}

void Projectile::_exit_tree() {
	RETURN_IF_EDITOR(void())

	m_fsm->deinit();
	delete m_fsm;
}

void Projectile::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	m_fsm->physicsProcess(delta);
}

void Projectile::_integrate_forces(godot::PhysicsDirectBodyState3D* state) {
	m_isOnGround = false;
	for (int i = 0; i < state->get_contact_count(); i++) {
		const auto normal = state->get_contact_local_normal(i);
		const bool onGround = normal.dot(g_up) > 0.99;
		m_isOnGround = onGround | m_isOnGround;
	}
}

void Projectile::onTimeout() {
	queue_free();
}
