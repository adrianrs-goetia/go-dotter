#pragma once

#include <core/core.h>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/timer.hpp>

namespace godot {
	class AudioStreamPlayer3D;
	class GPUParticles3D;
} //namespace godot

class TurretNode : public godot::Node3D {
	GDCLASS(TurretNode, godot::Node3D)

private:
	//
	godot::Node3D* m_gunRotPoint = nullptr; // Yaw rotation
	godot::Node3D* m_gunRotJoint = nullptr; // Pitch rotation
	godot::Node3D* m_gunOpening = nullptr; // Projectile spawn location
	godot::Node3D* m_target = nullptr;
	godot::Timer* m_firingTimer = nullptr;
	godot::AudioStreamPlayer3D* m_gunOpeningAudioPlayer = nullptr;
	godot::GPUParticles3D* m_gunOpeningGpuParticles = nullptr;

	godot::Ref<godot::PackedScene> m_projectileResource;

public:
	GETNAME(TurretNode)
	static void _bind_methods();
	//
	void _enter_tree() override;
	void _physics_process(double delta) override;

	float getFiringInterval() const;
	void rotateHeadTowardsTarget();
	void fireProjectile();

	godot::Vector3 getDirectionToTarget(const Node3D* source) const;
	godot::Vector3 getGunOpeningLocation() const;
	godot::Vector3 getGunOpeningDirection() const;

	struct paths {
		static godot::String gunRotPoint() { return "turret/GunRotCylinder/GunRotPoint"; }
		static godot::String gunRotJoint() { return gunRotPoint() + "/GunRotCylinder_001/GunRotJoint"; }
		static godot::String gunOpening() { return gunRotJoint() + "/GunRotCylinder_002/GunRotJoint_001/Gun"; }
	};
};