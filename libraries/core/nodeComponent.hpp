#pragma once

#include <core/core.hpp>

#include <functional>
#include <map>

#include <godot_cpp/classes/node3d.hpp>

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

#define GS_PATH_IMPL(variableName, functionName)                                                                       \
	void set##functionName(godot::NodePath value) {                                                                    \
		variableName = value;                                                                                          \
	}                                                                                                                  \
	godot::NodePath get##functionName() const {                                                                        \
		return variableName;                                                                                           \
	}
#define GS_BOOL_IMPL(variableName, functionName)                                                                       \
	void set##functionName(bool value) {                                                                               \
		variableName = value;                                                                                          \
	}                                                                                                                  \
	bool get##functionName() const {                                                                                   \
		return variableName;                                                                                           \
	}
#define GS_FLOAT_IMPL(variableName, functionName)                                                                      \
	void set##functionName(float value) {                                                                              \
		variableName = value;                                                                                          \
	}                                                                                                                  \
	float get##functionName() const {                                                                                  \
		return variableName;                                                                                           \
	}
#define GS_INT_IMPL(variableName, functionName)                                                                        \
	void set##functionName(int value) {                                                                                \
		variableName = value;                                                                                          \
	}                                                                                                                  \
	int get##functionName() const {                                                                                    \
		return variableName;                                                                                           \
	}
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

#define METHOD_PROPERTY_IMPL(class, functionName, propertyType, propertyName)                                          \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(get##functionName)), &class ::get##functionName);             \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(set##functionName), "value"), &class ::set##functionName);    \
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::propertyType, propertyName), TOSTRING(set##functionName),         \
			TOSTRING(get##functionName));
#define METHOD_PROPERTY_ENUM_IMPL(class, functionName, propertyType, propertyName, enumFields)                         \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(get##functionName)), &class ::get##functionName);             \
	godot::ClassDB::bind_method(godot::D_METHOD(TOSTRING(set##functionName), "value"), &class ::set##functionName);    \
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::propertyType, propertyName, PROPERTY_HINT_ENUM, enumFields),      \
			TOSTRING(set##functionName), TOSTRING(get##functionName));

/**
 * NodeComponent for Godot
 * Do NOT have more than one of a given NodeComponent type on a given Node
 */
class NodeComponent : public godot::Node3D {
	GDCLASS(NodeComponent, godot::Node3D)

public:
	using OnDestructionCb = std::function<void()>;
	enum DestructionId : uint16_t {
		GRAPPLECOMPONENT_INRANGE,
	};

private:
	bool m_isEnabled{ true };
	std::map<DestructionId, OnDestructionCb> m_onDestructionCbs;

protected:
	// Return true of state changed
	bool _setComponentEnabledImpl(bool enabled) {
		if (isComponentEnabled() == enabled) {
			return false;
		}
		m_isEnabled = enabled;
		return true;
	}
	virtual void _onExitTree() {
		for (auto&& [_, cb] : m_onDestructionCbs) {
			cb();
		}
	}

public:
	static void _bind_methods() {}

	void _exit_tree() {
		_onExitTree();
	}

	void addOnDestructionCb(DestructionId id, OnDestructionCb&& cb) {
		m_onDestructionCbs.insert({ id, std::move(cb) });
	}
	virtual void setComponentEnabled(bool enabled) = 0;
	bool isComponentEnabled() const {
		return m_isEnabled;
	}
};
