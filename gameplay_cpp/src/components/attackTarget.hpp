#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <events/attack.hpp>

#include <functional>

#include <godot_cpp/classes/node3d.hpp>

namespace godot {
class Area3D;
}

class ComponentAttackTarget : public NodeComponent {
	GDCLASS(ComponentAttackTarget, NodeComponent)

public:
	using ReceiveAttackCb = std::function<void(const EventAttack& instance)>;

	ReceiveAttackCb _callback;

public:
	static void _bind_methods() {}

	void setComponentEnabled(bool enabled) override {
		_setComponentEnabledImpl(enabled);
	}

	void receiveAttack(const EventAttack& instance) {
		if (_callback) {
			return _callback(instance);
		}
	}
};
