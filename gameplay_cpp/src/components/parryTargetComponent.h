#pragma once

#include <core/core.h>
#include <functional>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

class ParryInstance;

class ParryTargetComponent : public godot::Node3D {
	GDCLASS(ParryTargetComponent, godot::Node3D)

public:
	using OnParriedCb = std::function<void()>;

public:
	godot::NodePath m_pathToArea3D;
	godot::Area3D* m_areaPtr = nullptr;
	OnParriedCb m_onParriedCb = nullptr;
	bool m_isHeavy = false;
	float m_mass = 1.f;

public:
	GETNAME(ParryTargetComponent)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;

	void setAreaPath(godot::NodePath path);
	godot::NodePath getAreaPath() const;
	void setIsHeavy(bool isHeavy);
	bool getIsHeavy() const;
	void setMass(float mass);
	float getMass() const;

	void setOnParriedCb(OnParriedCb&& cb);

	void getParried(const ParryInstance& parryInstance);

	godot::Vector3 getPosition() const;
	godot::Vector3 getVelocity() const;
	godot::Vector3 getDesiredDirection() const;
};