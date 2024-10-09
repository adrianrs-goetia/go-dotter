#pragma once

#include <core/core.h>
#include <functional>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class ParryTargetComponent : public Node3D {
	GDCLASS(ParryTargetComponent, Node3D)

public:
	using OnParriedCb = std::function<void()>;

public:
	NodePath m_pathToArea3D;
	Area3D* m_areaPtr = nullptr;
	OnParriedCb m_onParriedCb = nullptr;

public:
	GETNAME(ParryTargetComponent)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;

	void setAreaPath(NodePath path);
	NodePath getAreaPath();

	void setOnParriedCb(OnParriedCb&& cb);

	void bindTest(Node3D* other) { LOG(DEBUG, "parried by", other->get_name()); }
};