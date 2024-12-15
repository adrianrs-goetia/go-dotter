#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <functional>

#include <godot_cpp/classes/node3d.hpp>

struct AttackInstance;

namespace godot {
class Area3D;
}

class AttackTargetComponent : public NodeComponent {
	GDCLASS(AttackTargetComponent, NodeComponent)

public:
	using ReceiveAttackCb = std::function<void(const AttackInstance& instance)>;

	// enum EAttacked {
	// 	NONE = -1,
	// };

	ReceiveAttackCb _callback;

public:
	static void _bind_methods() {}

	void setComponentEnabled(bool enabled) override {
		_setComponentEnabledImpl(enabled);
	}

	void receiveAttack(const AttackInstance& instance) {
		if (_callback) {
			return _callback(instance);
		}
	}
};