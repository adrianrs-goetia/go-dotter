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
	virtual void setComponentEnabled(bool enabled) {
		_setComponentEnabledImpl(enabled);
	}
	bool isComponentEnabled() const {
		return m_isEnabled;
	}
};
