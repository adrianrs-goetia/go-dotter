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
 * METHOD_..._IMPL in .cpp withing _bind_methods() definition
 */

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define GS_PATH_IMPL(variableName)                                                                                     \
	void set_##variableName##_Path(godot::NodePath path) { variableName = path; }                                      \
	godot::NodePath get_##variableName##_Path() const { return variableName; }

#define METHOD_PATH_IMPL(class, variableName, propertyName)                                                            \
	godot::ClassDB::bind_method(                                                                                       \
			godot::D_METHOD(TOSTRING(get_##variableName##_Path)), &class::get_##variableName##_Path);                 \
	godot::ClassDB::bind_method(                                                                                       \
			godot::D_METHOD(TOSTRING(set_##variableName##_Path), "path"), &class::set_##variableName##_Path);         \
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::NODE_PATH, propertyName), "setAreaPath", "getAreaPath");

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
		if (isComponentEnabled() == enabled) { return false; }
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

	void _exit_tree() { _onExitTree(); }

	void addOnDestructionCb(DestructionId id, OnDestructionCb&& cb) {
		m_onDestructionCbs.insert({ id, std::move(cb) });
	}
	virtual void setComponentEnabled(bool enabled) = 0;
	bool isComponentEnabled() const { return m_isEnabled; }
};
