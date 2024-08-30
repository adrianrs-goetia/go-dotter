#pragma once

#include <nodecomponent.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

using namespace godot;

struct GrappleEvent;

class GrappleComponent : public NodeComponent {
	GDCLASS(GrappleComponent, NodeComponent)
public:
	Area3D* m_area = nullptr;
	CollisionShape3D* m_shape = nullptr;

	bool anchored = true;
	float m_launch_strength = 10.f;
	float m_mass = 10.f;

public:
	void launch(GrappleComponent* subject);

protected:
	enum LaunchType {
		BOTH_ANCHOR,
		BOTH_NON_ANCHOR,
		INSTIGATOR_ANCHOR,
		SUBJECT_ANCHOR,
	};
	LaunchType _determine_launch_type(const GrappleComponent* subject);
	Vector3 _determine_launch_direction_atob(const GrappleComponent* a, const GrappleComponent* b);
	void _impulse_owner(const Vector3& direction, float launchstrengthmultiplier = 1.0f);

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;

	RID get_rid() const;

	void set_is_anchor(bool val);
	bool get_is_anchor() const;
	void set_launch_strength(float val);
	float get_launch_strength() const;
	void set_mass(float val);
	float get_mass() const;
};
