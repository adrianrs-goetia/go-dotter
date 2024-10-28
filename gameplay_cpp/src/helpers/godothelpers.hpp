#pragma onc

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>

/**
 * Helper functions or macros that relate to godot, but do not belong to any specific module or class
 */

// Distinction between editor-mode and in-game
#define RETURN_IF_EDITOR                                                                                               \
	if (godot::Engine::get_singleton()->is_editor_hint()) { return; }
#define RETURN_IF_EDITOR_RET(ret)                                                                                      \
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