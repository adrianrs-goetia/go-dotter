#include "projectile.h"

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

	m_stateContext.owner = this;
	m_fsm.setStateT<ProjectileFsm::Launched>(m_stateContext);

	m_parryTargetComp = getComponentOfNode<ParryTargetComponent>(this);
	// m_particles = getComponentOfNode<GPUParticles3D>(this);
	// m_audio = getComponentOfNode<AudioStreamPlayer3D>(this);

	ASSERT_NOTNULL(m_parryTargetComp)
	// ASSERT_NOTNULL(m_particles)
	// ASSERT_NOTNULL(m_audio)

	add_to_group(godotgroups::projectile);
}

void Projectile::_exit_tree() {
	RETURN_IF_EDITOR(void())

	m_fsm.deinit();
}

void Projectile::_physics_process(double delta) {}

void Projectile::_notification(int what) {
	switch (what) {
		case ENotifications::PARRIED:
		case ENotifications::ATTACKED:
		case ENotifications::DESTROY:
			m_fsm.notification(m_stateContext, what);
			break;
		default:
			break;
	}
}

void Projectile::onTimeout() {
	queue_free();
}
