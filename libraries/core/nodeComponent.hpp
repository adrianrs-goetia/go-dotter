#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/node3d.hpp>

/**
 * NodeComponent for Godot
 * Do NOT have more than one of a given NodeComponent type on a given Node
 */
class NodeComponent : public godot::Node3D {
	GDCLASS(NodeComponent, godot::Node3D)

public:
	static void _bind_methods() {}
};
