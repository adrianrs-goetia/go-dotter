#include <environment/grapplenode.h>

void GrappleNode::_bind_methods() { DEFAULT_PROPERTY(GrappleNode) }

void GrappleNode::_enter_tree() {
	// Node3D* node = new Node3D;
	// node->set_name("Collision");
	// add_child(node, true);
}

void GrappleNode::_exit_tree() {
	// queue_free();
}

void GrappleNode::grapplenode_in_range() { LOG(DEBUG, "Grapplenode is in range of player") }

void GrappleNode::grapplenode_out_of_range() { LOG(DEBUG, "Grapplenode is out of range for player") }
