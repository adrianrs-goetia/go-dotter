#ifndef GD_MAINNODE_PLUGIN_GAMEPLAY_H
#define GD_MAINNODE_PLUGIN_GAMEPLAY_H

#include <core/core.h>
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class MainNode : public Node {
	GDCLASS(MainNode, Node)

public:
	GETNAME(MainNode)
	static void _bind_methods();

	void _enter_tree() override;
};

#endif // GD_MAINNODE_PLUGIN_GAMEPLAY_H