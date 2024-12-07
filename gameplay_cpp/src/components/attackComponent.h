#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <functional>

#include <godot_cpp/classes/node3d.hpp>

namespace godot {
class Area3D;
}

class AttackComponent : public NodeComponent {
	GDCLASS(AttackComponent, NodeComponent)

public:
	enum EState {
		NONE = -1,
		NOT_HIT,
		HIT,
		PREVIOUSLY_HIT,
	};

public:
	godot::NodePath m_attackColliderPath;
	godot::Area3D* m_attackCollider = nullptr;

	// EState m_state;
	uint8_t m_numOfHitNodes = 0;
	uint8_t m_numOfHitNodesTotal = 0;

public:
	static void _bind_methods();

	void setComponentEnabled(bool enabled) override;

	void _enter_tree();
	void _exit_tree() override;

	void areaEnteredCollider(godot::Area3D* area);
	void areaExitedCollider(godot::Area3D* area);

	EState getAttackState() const;

	GS_PATH_IMPL(m_attackColliderPath, ColliderPath)
	// GS_ENUM_IMPL(m_state, AttackState, EState)
};