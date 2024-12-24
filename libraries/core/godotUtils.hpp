#pragma once

#include "assert.hpp"
#include "log.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

// Directions
static const godot::Vector3 g_up(0, 1, 0);
static const godot::Vector3 g_forward(0, 0, 1);
static const godot::Vector3 g_right(1, 0, 0);

/**
 * Helper functions or macros that relate to godot, but do not belong to any specific module or class
 */

// Distinction between editor-mode and in-game
#define RETURN_IF_EDITOR(ret)                                                                                          \
	if (godot::Engine::get_singleton()->is_editor_hint()) {                                                            \
		return ret;                                                                                                    \
	}

#define RETURN_IF_GAME(ret)                                                                                            \
	if (!godot::Engine::get_singleton()->is_editor_hint()) {                                                           \
		return ret;                                                                                                    \
	}

/**
 * We ALWAYS expect there to only be a single component of type T under a specific node
 */
template <typename T>
T* getComponentOfNode(const godot::Node* node) {
	godot::TypedArray<godot::Node> children = node->get_children();
	int num = 0;
	T* foundNode = nullptr;
	for (int i = 0; i < children.size(); ++i) {
		if (T* child = node->cast_to<T>(children[i])) {
			++num;
			foundNode = child;
		}
	}
	ASSERT(num <= 1)
	return foundNode;
}

template <typename T>
T* getAdjacentNode(const godot::Node* node) {
	if (!node->get_parent()) {
		LOG(ERROR, "Node did not have parent: ", node->get_path())
	}
	if (godot::Node* parent = node->get_parent()) {
		return getComponentOfNode<T>(parent);
	}
	return nullptr;
}

/**
 * Asserts parent of node is of a given type
 */
template <typename T>
T* getParentNode(const godot::Node* node) {
	if (!node->get_parent()) {
		LOG(ERROR, "Node did not have parent: ", node->get_path())
	}
	if (T* parent = node->cast_to<T>(node->get_parent())) {
		return parent;
	}
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

inline auto getHorizontalUnit(const godot::Vector3& vec) {
	return godot::Vector3(vec.x, 0, vec.z).normalized();
}

/**
 * Common property implementations as macros
 * Usually just getters and setters causing a bunch of boilerplate code
 * GS == getter setter
 *
 * GS_..._IMPL in .h
 * METHOD_PROPERTY_IMPL in .cpp withing _bind_methods() definition
 */

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define _GS_IMPL_VAR(variableName, type, ...)                                                                          \
protected:                                                                                                             \
	type variableName __VA_ARGS__;

#define _G_IMPL(variableName, functionName, type, ...)                                                                 \
	_GS_IMPL_VAR(variableName, type, __VA_ARGS__)                                                                      \
public:                                                                                                                \
	type get##functionName() const {                                                                                   \
		return variableName;                                                                                           \
	}

#define _S_IMPL(variableName, functionName, type, ...)                                                                 \
	_GS_IMPL_VAR(variableName, type, __VA_ARGS__)                                                                      \
public:                                                                                                                \
	void set##functionName(type value) {                                                                               \
		variableName = value;                                                                                          \
	}

#define _GS_IMPL(variableName, functionName, type, ...)                                                                \
	_GS_IMPL_VAR(variableName, type, __VA_ARGS__)                                                                      \
public:                                                                                                                \
	type get##functionName() const {                                                                                   \
		return variableName;                                                                                           \
	}                                                                                                                  \
	void set##functionName(type value) {                                                                               \
		variableName = value;                                                                                          \
	}

#define G_PATH_IMPL(variableName, functionName, ...) _G_IMPL(variableName, functionName, godot::NodePath, __VA_ARGS__)
#define S_PATH_IMPL(variableName, functionName, ...) _S_IMPL(variableName, functionName, godot::NodePath, __VA_ARGS__)
#define GS_PATH_IMPL(variableName, functionName, ...) _GS_IMPL(variableName, functionName, godot::NodePath, __VA_ARGS__)

#define G_BOOL_IMPL(variableName, functionName, ...) _G_IMPL(variableName, functionName, bool, __VA_ARGS__)
#define S_BOOL_IMPL(variableName, functionName, ...) _S_IMPL(variableName, functionName, bool, __VA_ARGS__)
#define GS_BOOL_IMPL(variableName, functionName, ...) _GS_IMPL(variableName, functionName, bool, __VA_ARGS__)

#define G_FLOAT_IMPL(variableName, functionName, ...) _G_IMPL(variableName, functionName, float, __VA_ARGS__)
#define S_FLOAT_IMPL(variableName, functionName, ...) _S_IMPL(variableName, functionName, float, __VA_ARGS__)
#define GS_FLOAT_IMPL(variableName, functionName, ...) _GS_IMPL(variableName, functionName, float, __VA_ARGS__)

#define G_INT_IMPL(variableName, functionName, ...) _G_IMPL(variableName, functionName, int, __VA_ARGS__)
#define S_INT_IMPL(variableName, functionName, ...) _S_IMPL(variableName, functionName, int, __VA_ARGS__)
#define GS_INT_IMPL(variableName, functionName, ...) _GS_IMPL(variableName, functionName, int, __VA_ARGS__)

#define G_PACKEDSCENE_IMPL(variableName, functionName)                                                                 \
	_G_IMPL(variableName, functionName, godot::Ref<godot::PackedScene>)
#define S_PACKEDSCENE_IMPL(variableName, functionName)                                                                 \
	_S_IMPL(variableName, functionName, godot::Ref<godot::PackedScene>)
#define GS_PACKEDSCENE_IMPL(variableName, functionName)                                                                \
	_GS_IMPL(variableName, functionName, godot::Ref<godot::PackedScene>)

#define GS_ENUM_IMPL(variableName, functionName, enumType)                                                             \
	void set##functionName(int value) {                                                                                \
		variableName = static_cast<enumType>(value);                                                                   \
	}                                                                                                                  \
	int get##functionName() const {                                                                                    \
		return static_cast<int>(variableName);                                                                         \
	}                                                                                                                  \
	enumType get##functionName##Enum() const {                                                                         \
		return variableName;                                                                                           \
	}

#define METHOD_PROPERTY_IMPL(class, functionName, propertyType)                                                        \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(get##functionName)), &class ::get##functionName);             \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(set##functionName), "value"), &class ::set##functionName);    \
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::propertyType, TOSTRING(functionName)),                            \
		TOSTRING(set##functionName),                                                                                   \
		TOSTRING(get##functionName));
#define METHOD_PROPERTY_ENUM_IMPL(class, functionName, propertyType, enumFields)                                       \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(get##functionName)), &class ::get##functionName);             \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(set##functionName), "value"), &class ::set##functionName);    \
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::propertyType,                                                     \
					 TOSTRING(functionName),                                                                           \
					 godot::PropertyHint::PROPERTY_HINT_ENUM,                                                          \
					 enumFields),                                                                                      \
		TOSTRING(set##functionName),                                                                                   \
		TOSTRING(get##functionName));
#define METHOD_PROPERTY_PACKEDSCENE_IMPL(class, functionName)                                                          \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(get##functionName)), &class ::get##functionName);             \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(set##functionName), "value"), &class ::set##functionName);    \
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT,                                                           \
					 TOSTRING(functionName),                                                                           \
					 godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,                                                 \
					 "PackedScene"),                                                                                   \
		TOSTRING(set##functionName),                                                                                   \
		TOSTRING(get##functionName));

#define METHOD_INOUT_BIND(classname, entered, exited, fieldName)                                                       \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(entered), TOSTRING(fieldName)), &classname::entered);         \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(exited), TOSTRING(fieldName)), &classname::exited);
