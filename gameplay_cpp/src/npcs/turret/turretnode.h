#pragma once

#include <core/core.h>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

using namespace godot;

class TurretNode : public Node3D {
	GDCLASS(TurretNode, Node3D)

public:
	GETNAME(TurretNode)
	static void _bind_methods();

	//
	Node3D* m_gunRotPoint = nullptr; // Yaw rotation
	Node3D* m_gunRotJoint = nullptr; // Pitch rotation
	Node3D* m_gunOpening = nullptr; // Projectile spawn location
	Node3D* m_target = nullptr;

	// @todo. PLACE THIS IN A RESOURCE LIBRARY FOR GAMEPLAY_CPP
	// each instance of a turret loading a resource is pretty shit,
	// unless godot::ResourceLoader does some caching...
	Ref<PackedScene> m_projectileResource;

	float m_firingTimer = 0.f;

	//
	void _enter_tree() override;
	void _physics_process(double delta) override;

	void rotate_head_towards_target();
	void fire_projectile();

	Vector3 getDirectionToTarget(const Node3D* source) const;
	Vector3 getGunOpeningLocation() const;
	Vector3 getGunOpeningDirection() const;

	struct paths {
		static std::string gunRotPoint() { return "turret/GunRotCylinder/GunRotPoint"; }
		static std::string gunRotJoint() { return gunRotPoint() + "/GunRotCylinder_001/GunRotJoint"; }
		static std::string gunOpening() { return gunRotJoint() + "/GunRotCylinder_002/GunRotJoint_001/Gun"; }
	};
};