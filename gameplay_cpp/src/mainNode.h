#ifndef GD_MAINNODE_PLUGIN_GAMEPLAY_H
#define GD_MAINNODE_PLUGIN_GAMEPLAY_H

#include <core/core.h>
#include <godot_cpp/classes/node.hpp>

class MainNode : public godot::Node {
	GDCLASS(MainNode, godot::Node)

public:
	static void _bind_methods();

	void _enter_tree() override;
};

#endif // GD_MAINNODE_PLUGIN_GAMEPLAY_H