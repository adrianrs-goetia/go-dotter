#include "projectile.h"

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

	set_contact_monitor(true);
	set_max_contacts_reported(3);

	m_stateContext.owner = this;
	m_fsm.setStateT<fsm::projectile::TLaunched>(m_stateContext);

	m_parryTargetComp = getComponentOfNode<ParryTargetComponent>(this);
	m_attackTargetComp = memnew(AttackTargetComponent);
	add_child(m_attackTargetComp);
	// m_particles = getComponentOfNode<GPUParticles3D>(this);
	// m_audio = getComponentOfNode<AudioStreamPlayer3D>(this);

	ASSERT_NOTNULL(m_parryTargetComp)
	ASSERT_NOTNULL(m_attackTargetComp)
	// ASSERT_NOTNULL(m_particles)
	// ASSERT_NOTNULL(m_audio)

	m_parryTargetComp->m_onParriedCb = std::bind(&Projectile::onParried, this, std::placeholders::_1);
	m_attackTargetComp->_callback = std::bind(&Projectile::onAttacked, this, std::placeholders::_1);

	add_to_group(godotgroups::projectile);
}

void Projectile::_exit_tree() {
	RETURN_IF_EDITOR(void())

	m_fsm.deinit();
}

void Projectile::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	m_fsm.physicsProcess(m_stateContext, delta);
}

void Projectile::_integrate_forces(godot::PhysicsDirectBodyState3D* state) {
	m_isOnGround = false;
	for (int i = 0; i < state->get_contact_count(); i++){
		const auto normal = state->get_contact_local_normal(i);
		const bool onGround = normal.dot(g_up) > 0.99;
		m_isOnGround = onGround | m_isOnGround;
	}
}

void Projectile::onTimeout() {
	queue_free();
}

void Projectile::onAttacked(const AttackInstance& instance) {
	m_fsm.handleExternalAction(m_stateContext, instance);
}

void Projectile::onParried(const ParryInstance& instance) {
	m_fsm.handleExternalAction(m_stateContext, instance);
}
