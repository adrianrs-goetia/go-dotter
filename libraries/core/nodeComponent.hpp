#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/node3d.hpp>

/**
 * NodeComponent for Godot
 * Do NOT have more than one of a given NodeComponent type on a given Node
 */
class NodeComponent : public godot::Node3D {
	GDCLASS(NodeComponent, godot::Node3D)

	bool m_isEnabled{ true };

protected:
	bool _setComponentEnabledImpl(bool enabled) {
		if (isComponentEnabled() == enabled) { return false; }
		m_isEnabled = enabled;
		return true;
	}

public:
	static void _bind_methods() {}

	virtual void setComponentEnabled(bool enabled) = 0;
	bool isComponentEnabled() const { return m_isEnabled; }
};
