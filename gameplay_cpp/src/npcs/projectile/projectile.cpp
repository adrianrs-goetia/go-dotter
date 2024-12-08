#include "projectile.h"

#include <components/parryTargetComponent.h>

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>

#include <configHandler.h>

#define CONFIG_PREFIX "npcs", "projectile"

using namespace godot;

void Projectile::_bind_methods() {}

void Projectile::_enter_tree() {
	RETURN_IF_EDITOR(void())
	//

	m_timer = memnew(Timer);
	m_parryTargetComp = getChildOfNode<ParryTargetComponent>(this);
	m_particles = getChildOfNode<GPUParticles3D>(this);
	// m_audio = getChildOfNode<AudioStreamPlayer3D>(this);

	ASSERT_NOTNULL(m_timer)
	ASSERT_NOTNULL(m_parryTargetComp)
	ASSERT_NOTNULL(m_particles)
	// ASSERT_NOTNULL(m_audio)

	add_child(m_timer);
	m_timer->connect("timeout", callable_mp(this, &Projectile::onTimeout));
	m_timer->start(GETPARAM_D("lifetime"));

	add_to_group(godotgroups::projectile);
}

void Projectile::_physics_process(double delta) {}

void Projectile::_notification(int what) {
	switch (what) {
		case ENotifications::ATTACKED: {
			m_particles->restart();
			queue_free();
		}
		case ENotifications::DESTROY: {
			queue_free();
			break;
		}
		default:
			break;
	}
}

void Projectile::onTimeout() {
	queue_free();
}
