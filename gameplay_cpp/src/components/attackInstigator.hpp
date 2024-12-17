#pragma once

#include "attackTarget.hpp"
#include <core/core.hpp>
#include <core/nodeComponent.hpp>
#include <events/attack.hpp>

#include <functional>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include <debugdraw3d/api.h>

class ComponentAttackInstigator : public NodeComponent {
	GDCLASS(ComponentAttackInstigator, NodeComponent)

public:
	enum EState {
		NONE = -1,
		NOT_HIT,
		HIT,
		PREVIOUSLY_HIT,
	};

private:
	GS_PATH_IMPL(m_attackColliderPath, ColliderPath)
	GS_FLOAT_IMPL(m_attackStrength, AttackStrength);

	godot::Area3D* m_attackCollider = nullptr;

	uint8_t m_numOfHitNodes = 0;
	uint8_t m_numOfHitNodesTotal = 0;

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(ComponentAttackInstigator, ColliderPath, NODE_PATH)
		METHOD_INOUT_BIND(ComponentAttackInstigator, areaEnteredCollider, areaExitedCollider, area)
	}

	void setComponentEnabled(bool enabled) override {
		if (!_setComponentEnabledImpl(enabled)) {
			return;
		}

		if (m_attackCollider) {
			m_attackCollider->set_monitoring(enabled);
		}
		if (enabled) {
			m_numOfHitNodes = 0;
			m_numOfHitNodesTotal = 0;
		}
	}

	void _enter_tree() {
		RETURN_IF_EDITOR(void())

		m_attackCollider = get_node<Area3D>(m_attackColliderPath);
		ASSERT_NOTNULL(m_attackCollider)

		m_attackCollider->set_collision_mask_value(collisionflags::attackTarget, true);
		m_attackCollider->connect("area_entered", callable_mp(this, &ComponentAttackInstigator::areaEnteredCollider));
		m_attackCollider->connect("area_exited", callable_mp(this, &ComponentAttackInstigator::areaExitedCollider));

		setComponentEnabled(false);
	}

	void areaEnteredCollider(godot::Area3D* area) {
		/**
		 * Area is assumed to not be monitoring while attack component is not enabled
		 * This callback assumes ComponentAttackInstigator is enabled
		 */

		++m_numOfHitNodes;
		++m_numOfHitNodesTotal;

		auto* target = area->get_parent();
		ASSERT_NOTNULL(target);

		if (auto* attackComp = getComponentOfNode<ComponentAttackTarget>(target)) {
			attackComp->receiveAttack(
				EventAttack{ get_global_transform(), attackComp->get_global_transform(), getAttackStrength() });
		}
	}

	void areaExitedCollider(godot::Area3D* area) {
		--m_numOfHitNodes;
	}

	EState getAttackState() const {
		if (m_numOfHitNodes > 0) {
			return EState::HIT;
		}
		else if (m_numOfHitNodes == 0 && m_numOfHitNodesTotal > 0) {
			return EState::PREVIOUSLY_HIT;
		}
		else if (m_numOfHitNodes == 0) {
			return EState::NOT_HIT;
		}
		ASSERT(false)
		return EState::NONE; // Should never happen
	}
};
