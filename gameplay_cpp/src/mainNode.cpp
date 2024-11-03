#include <mainNode.h>

using namespace godot;

void MainNode::_bind_methods() {}

void MainNode::_enter_tree() {
	// Name is magic value for path that is hard coded in core module
	set_name("Mainscene");
}
