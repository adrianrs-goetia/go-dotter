#pragma once

#include <core/core.hpp>

#include <functional>

#include <godot_cpp/classes/node3d.hpp>

// /**
//  * Entry for keeping raw pointers to Nodes that are not refcounted by godot
//  */
// template <typename T>
// struct RawPointerEntry {
// 	T* ptr{ nullptr };
// };

/**
 * NodeComponent for Godot
 * Do NOT have more than one of a given NodeComponent type on a given Node
 */
class NodeComponent : public godot::Node3D {
	GDCLASS(NodeComponent, godot::Node3D)

public:
	using OnDestructionCb = std::function<void()>;

private:
	bool m_isEnabled{ true };
	std::vector<OnDestructionCb> m_onDestructionCbs;

protected:
	// Return true of state changed
	bool _setComponentEnabledImpl(bool enabled) {
		if (isComponentEnabled() == enabled) { return false; }
		m_isEnabled = enabled;
		return true;
	}
	virtual void _onExitTree() {
		for (auto&& cb : m_onDestructionCbs) {
			cb();
		}
	}

public:
	static void _bind_methods() {}

	void _exit_tree() { _onExitTree(); }

	void addOnDestructionCb(OnDestructionCb&& cb) { m_onDestructionCbs.emplace_back(std::move(cb)); }
	virtual void setComponentEnabled(bool enabled) = 0;
	bool isComponentEnabled() const { return m_isEnabled; }
};
