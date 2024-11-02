#pragma once

#include <core/nodeComponent.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

using namespace godot;

struct GrappleEvent;

class GrappleComponent : public NodeComponent {
	GDCLASS(GrappleComponent, NodeComponent)
public:
	NodePath m_pathToArea3D;
	Area3D* m_area = nullptr;

	bool m_anchored = false;
	float m_pullStrength = 0.f;
	float m_mass = 10.f;

public:
	enum LaunchType {
		BOTH_ANCHOR,
		BOTH_NON_ANCHOR,
		INSTIGATOR_ANCHOR,
		SUBJECT_ANCHOR,
	};
	struct LaunchContext {
		Vector3 impulse;
		LaunchType type;
	};

public:
	LaunchContext launch(GrappleComponent* subject);

protected:
	LaunchType _determineLaunchType(const GrappleComponent* subject);
	Vector3 _determineLaunchDirectionAtob(const GrappleComponent* a, const GrappleComponent* b);
	Vector3 _impulseOwner(const Vector3& direction, float impulse_strength);

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;

	void setAreaPath(NodePath path);
	NodePath getAreaPath();

	RID getRid() const;

	void setIsAnchor(bool val);
	bool getIsAnchor() const;
	void setPullStrength(float val);
	float getPullStrength() const;
	void setMass(float val);
	float getMass() const;
};
