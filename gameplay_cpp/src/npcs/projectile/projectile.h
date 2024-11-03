#pragma once

#include <core/core.h>

#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/timer.hpp>

class Projectile : public godot::RigidBody3D {
	GDCLASS(Projectile, godot::RigidBody3D)
private:
	godot::Timer* m_timer = nullptr;

public:
	static void _bind_methods();

	//
	void _enter_tree() override;
	void _physics_process(double delta) override;

	void onTimeout();
};