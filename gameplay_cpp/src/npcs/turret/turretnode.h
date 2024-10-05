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
	Node3D* m_turretHead = nullptr;
	Node3D* m_target = nullptr;

	Ref<PackedScene> m_projectileResource;

	float m_firingTimer = 0.f;

	//
	void _enter_tree() override;
	void _physics_process(double delta) override;

	void rotate_head_towards_target(float rotationSpeed);
	void fire_projectile();

    Vector3 getDirectionToTarget() const;
};