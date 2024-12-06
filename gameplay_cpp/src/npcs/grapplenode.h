#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/node3d.hpp>

class ParryTargetComponent;

class GrappleNode : public godot::Node3D {
	GDCLASS(GrappleNode, godot::Node3D)

public:
	static void _bind_methods();

	//
	void _enter_tree() override;
};