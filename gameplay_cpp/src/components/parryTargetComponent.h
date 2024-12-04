#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <functional>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

class ParryInstance;

class ParryTargetComponent : public NodeComponent {
	GDCLASS(ParryTargetComponent, NodeComponent)

public:
	using OnParriedCb = std::function<void()>;

public:
	godot::NodePath m_pathToArea3D;
	godot::Area3D* m_areaPtr = nullptr;
	OnParriedCb m_onParriedCb = nullptr;
	bool m_isHeavy = false;
	float m_mass = 1.f;

public:
	static void _bind_methods();

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() override;
	void _exit_tree() override;

	void setOnParriedCb(OnParriedCb&& cb);

	void getParried(const ParryInstance& parryInstance);

	godot::Vector3 getPosition() const;
	godot::Vector3 getVelocity() const;
	godot::Vector3 getDesiredDirection() const;


	GS_PATH_IMPL(m_pathToArea3D, AreaPath)
	GS_BOOL_IMPL(m_isHeavy, IsHeavy)
	GS_FLOAT_IMPL(m_mass, Mass)
};