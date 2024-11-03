#pragma once

#include <core/nodeComponent.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

class GrappleTargetComponent : public NodeComponent {
	GDCLASS(GrappleTargetComponent, NodeComponent)

public:
	enum LaunchType {
		BOTH_ANCHOR,
		BOTH_NON_ANCHOR,
		INSTIGATOR_ANCHOR,
		SUBJECT_ANCHOR,
	};
	struct LaunchContext {
		godot::Vector3 impulse;
		LaunchType type;
	};

private:
	godot::NodePath m_pathToArea3D;
	godot::Area3D* m_area = nullptr;

	bool m_anchored = false;
	float m_pullStrength = 0.f;
	float m_mass = 10.f;

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;

	LaunchContext launch(GrappleTargetComponent* subject);

	godot::RID getRid() const;

protected:
	LaunchType _determineLaunchType(const GrappleTargetComponent* subject);
	godot::Vector3 _determineLaunchDirectionAtob(const GrappleTargetComponent* a, const GrappleTargetComponent* b);
	godot::Vector3 _impulseOwner(const godot::Vector3& direction, float impulse_strength);

public: // get/set editor-properties
	void setAreaPath(godot::NodePath path);
	godot::NodePath getAreaPath();
	void setIsAnchor(bool anchored);
	bool getIsAnchor() const;
	void setPullStrength(float pullStrength);
	float getPullStrength() const;
	void setMass(float mass);
	float getMass() const;
};
