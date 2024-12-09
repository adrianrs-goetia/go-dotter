#pragma once

#include <core/core.hpp>

#include "fsm.h"

#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

class ParryTargetComponent;

namespace godot {
class AudioStreamPlayer3D;
class GPUParticles3D;
} //namespace godot

class Projectile : public godot::RigidBody3D {
	GDCLASS(Projectile, godot::RigidBody3D)
private:
	ProjectileFsm::StateContext m_stateContext;
	ProjectileFsm::Fsm m_fsm;

	// godot::AudioStreamPlayer3D* m_audio = nullptr; // todo, audiocomponent for streaming multiple sounds from
	// context?
	// godot::GPUParticles3D* m_particles = nullptr; // todo, same as above?
	ParryTargetComponent* m_parryTargetComp = nullptr;
	godot::Ref<godot::PackedScene> m_deathParticles;

public:
	static void _bind_methods();

	//
	void _enter_tree() override;
	void _exit_tree() override;
	void _physics_process(double delta) override;
	void _notification(int what);

	void onTimeout();


	GS_PACKEDSCENE_IMPL(m_deathParticles, DeathParticles)
};