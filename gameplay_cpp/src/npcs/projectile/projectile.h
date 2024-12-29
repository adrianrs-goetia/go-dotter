#pragma once

#include <core/core.hpp>

#include "fsm/typedefs.hpp"

#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

class ComponentAttackTarget;
struct EventAttack;
class ComponentParryTarget;
class EventParry;

namespace fsm::projectile {
class Fsm;
}

namespace godot {
class AudioStreamPlayer3D;
class GPUParticles3D;
} //namespace godot

class Projectile : public godot::RigidBody3D {
	GDCLASS(Projectile, godot::RigidBody3D)

private:
	GS_PACKEDSCENE_IMPL(m_deathParticles, DeathParticles)

	fsm::projectile::Fsm* m_fsm = nullptr;

	// godot::AudioStreamPlayer3D* m_audio = nullptr; // todo, audiocomponent for streaming multiple sounds from
	// godot::GPUParticles3D* m_particles = nullptr; // todo, same as above?
	ComponentParryTarget* m_parryTargetComp = nullptr;
	ComponentAttackTarget* m_attackTargetComp = nullptr;

	bool m_isOnGround = false;

public:
	static void _bind_methods();

	//
	void _enter_tree() override;
	void _exit_tree() override;
	void _physics_process(double delta) override;
	void _integrate_forces(godot::PhysicsDirectBodyState3D* state) override;

	void handleExternalEvent(fsm::projectile::VExternalEvent action);

	bool isOnGround() const {
		return m_isOnGround;
	}

	void onTimeout();

	ComponentParryTarget& getCompParry() const {
		ASSERTNN(m_parryTargetComp)
		return *m_parryTargetComp;
	}
};