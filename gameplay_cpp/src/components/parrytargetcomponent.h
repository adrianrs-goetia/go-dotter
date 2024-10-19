#pragma once

#include <core/core.h>
#include <functional>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class ParryInstance;

class ParryTargetComponent : public Node3D {
	GDCLASS(ParryTargetComponent, Node3D)

public:
	using OnParriedCb = std::function<void()>;

public:
	NodePath m_pathToArea3D;
	Area3D* m_areaPtr = nullptr;
	OnParriedCb m_onParriedCb = nullptr;
	bool m_isHeavy = false;
	float m_mass = 1.f;

public:
	GETNAME(ParryTargetComponent)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;

	void setAreaPath(NodePath path);
	NodePath getAreaPath() const;
	void setIsHeavy(bool isHeavy);
	bool getIsHeavy() const;
	void setMass(float mass);
	float getMass() const;

	void setOnParriedCb(OnParriedCb&& cb);

	void getParried(const ParryInstance& parryInstance);

	Vector3 getPosition() const;
	Vector3 getVelocity() const;
	Vector3 getDesiredDirection() const;
};