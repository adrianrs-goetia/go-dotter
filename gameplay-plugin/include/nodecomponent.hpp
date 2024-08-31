#pragma once

#include <core/core.h>

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

/**
 * NodeComponent for Godot
 * Do NOT have more than one of a given NodeComponent type on a given Node
 */
class NodeComponent : public Node3D {
	GDCLASS(NodeComponent, Node3D)

public:
	static void _bind_methods() {}

	/**
	 * Looks after component of type
	 * Will only search immediate children, not-recursively
	 * returns first NodeComponent of type
	 */
	template <typename T>
	static T* get_child_node_component(const Node* node);

	/**
	 * Given a component, find other components on a given Node.
	 * Will only look for components at the same depth as the current component
	 */
	template <typename T>
	T* get_adjacent_node_component() const;

	/**
	 * Asserts parent derives from class T
	 * Tmp method for potensially making it easier to fetch recursively or certain amount of steps up in the future
	 */
	template <typename T>
	T* get_parent_node() const;
};

//////////////////////////////////////////
// Method definitions
//////////////////////////////////////////
template <typename T>
T* NodeComponent::get_child_node_component(const Node* node) {
	ASSERT(node != nullptr, "")
	TypedArray<Node> children = node->get_children(true);
	for (int i = 0; i < children.size(); ++i) {
		if (T* child = cast_to<T>(children[i])) { return child; }
	}
	return nullptr;
}

template <typename T>
T* NodeComponent::get_adjacent_node_component() const {
	if (const Node* parent = get_parent()) { return NodeComponent::get_child_node_component<T>(parent); }
	return nullptr;
}

template <typename T>
T* NodeComponent::get_parent_node() const {
	if (T* parent = cast_to<T>(get_parent())) { return parent; }
	return nullptr;
}
