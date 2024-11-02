#pragma onc

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>

/**
 * Helper functions or macros that relate to godot, but do not belong to any specific module or class
 */

// Distinction between editor-mode and in-game
#define RETURN_IF_EDITOR(ret)                                                                                          \
	if (godot::Engine::get_singleton()->is_editor_hint()) { return ret; }

#define GETNAME(class_name)                                                                                            \
	String get_class_name() const { return #class_name; }
#define DEFAULT_PROPERTY(class_name)                                                                                   \
	godot::ClassDB::bind_method(D_METHOD("get_class_name"), &class_name::get_class_name);                              \
	godot::ClassDB::add_property(#class_name,                                                                          \
			PropertyInfo(Variant::STRING, "class_name", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "",           \
			"get_class_name");

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
	if (godot::Node* parent = node->get_parent()) { return getChildOfNode<T>(parent); }
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