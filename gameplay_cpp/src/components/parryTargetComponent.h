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

	enum EParryTargetTag {
		NONE = -1,
		DYNAMIC_LIGHT = 0,
		DYNAMIC_HEAVY = 1,
		STATIC_OBJECT = 2,
		STATIC_TERRAIN = 3,
	};

public:
	godot::NodePath m_colliderPath;
	godot::Area3D* m_areaPtr = nullptr;
	OnParriedCb m_onParriedCb = nullptr;
	float m_mass = 1.f;
	EParryTargetTag m_parryTag = EParryTargetTag::NONE;

public:
	static void _bind_methods();

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree();
	void _exit_tree() override;

	void setOnParriedCb(OnParriedCb&& cb);

	void getParried(const ParryInstance& parryInstance);

	godot::Vector3 getPosition() const;
	godot::Vector3 getVelocity() const;
	godot::Vector3 getDesiredDirection() const;

	GS_PATH_IMPL(m_colliderPath, ColliderPath)
	GS_FLOAT_IMPL(m_mass, Mass)
	GS_ENUM_IMPL(m_parryTag, ParryTag, EParryTargetTag)
};