#pragma once

#include <core/core.h>

#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/timer.hpp>

using namespace godot;

class Projectile : public RigidBody3D {
	GDCLASS(Projectile, RigidBody3D)
private:
	Timer* m_timer = nullptr;

public:
	GETNAME(TurretNode)
	static void _bind_methods();

	//
	void _enter_tree() override;
	void _physics_process(double delta) override;

	void onTimeout();
};