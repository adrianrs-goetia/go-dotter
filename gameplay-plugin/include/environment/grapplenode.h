#ifndef GD_ENVIRONMENT_GRAPPLENODE_PLUGIN_GAMEPLAY_H
#define GD_ENVIRONMENT_GRAPPLENODE_PLUGIN_GAMEPLAY_H

#include <core/core.h>
#include <godot_cpp/classes/area3d.hpp>

using namespace godot;

class GrappleNode : public Area3D {
	GDCLASS(GrappleNode, Area3D)

public:
public:
	GETNAME(GrappleNode)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;

	void grapplenode_in_range();
	void grapplenode_out_of_range();
};

#endif // GD_ENVIRONMENT_GRAPPLENODE_PLUGIN_GAMEPLAY_H