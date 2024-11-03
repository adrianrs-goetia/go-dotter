#pragma once

#include "log.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>

// Directions
static const godot::Vector3 g_up(0, 1, 0);
static const godot::Vector3 g_forward(0, 0, 1);
static const godot::Vector3 g_right(1, 0, 0);

/**
 * Helper functions or macros that relate to godot, but do not belong to any specific module or class
 */

// Distinction between editor-mode and in-game
#define RETURN_IF_EDITOR(ret)                                                                                          \
	if (godot::Engine::get_singleton()->is_editor_hint()) { return ret; }

template <typename T>
T* getChildOfNode(const godot::Node* node) {
	godot::TypedArray<godot::Node> children = node->get_children();
	for (int i = 0; i < children.size(); ++i) {
		if (T* child = node->cast_to<T>(children[i])) { return child; }
	}
	return nullptr;
}

template <typename T>
T* getAdjacentNode(const godot::Node* node) {
	if (!node->get_parent()) { LOG(ERROR, "Node did not have parent: ", node->get_path()) }
	if (godot::Node* parent = node->get_parent()) { return getChildOfNode<T>(parent); }
	return nullptr;
}

/**
 * Asserts parent of node is of a given type
 */
template <typename T>
T* getParentNode(const godot::Node* node) {
	if (!node->get_parent()) { LOG(ERROR, "Node did not have parent: ", node->get_path()) }
	if (T* parent = node->cast_to<T>(node->get_parent())) { return parent; }
	return nullptr;
}

/**
 * Get basis pointing towards a direction
 * Up is going towards global up
 * Assumes that direction is a unit vector
 */
inline godot::Basis createBasisFromDirection(
		const godot::Vector3& direction, const godot::Vector3& scale = godot::Vector3(1, 1, 1)) {
	godot::Basis basis;
	const godot::Vector3 right = g_up.cross(direction);
	basis.set_column(0, right * scale.x); // x - right
	basis.set_column(1, direction.cross(right) * scale.y); // y - up
	basis.set_column(2, direction * scale.z); // z - forward
	return basis;
}

inline godot::Vector3 getScaleFromBasis(const godot::Basis& basis) {
	return godot::Vector3(basis.get_column(0).length(), basis.get_column(1).length(), basis.get_column(2).length());
}