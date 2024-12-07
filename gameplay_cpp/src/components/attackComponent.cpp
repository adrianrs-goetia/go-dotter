#include "attackComponent.h"

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

using namespace godot;

// VARIANT_ENUM_CAST(AttackComponent::EState)

void AttackComponent::_bind_methods() {
	// BIND_ENUM_CONSTANT(NOT_HIT)
	// BIND_ENUM_CONSTANT(HIT)
	// METHOD_PROPERTY_ENUM_IMPL(AttackComponent, AttackState, INT, "NOT_HIT,HIT")

	METHOD_PROPERTY_IMPL(AttackComponent, ColliderPath, NODE_PATH)

	BIND_INOUT_METHODS(AttackComponent, areaEnteredCollider, areaExitedCollider, area)
}

void AttackComponent::setComponentEnabled(bool enabled) {
	if (!_setComponentEnabledImpl(enabled)) {
		return;
	}

	m_numOfHitNodes = 0;
	m_numOfHitNodesTotal = 0;
}

void AttackComponent::_enter_tree() {
	RETURN_IF_EDITOR(void())

	m_attackCollider = get_node<Area3D>(m_attackColliderPath);
	ASSERT_NOTNULL(m_attackCollider)

	m_attackCollider->set_collision_mask_value(collisionflags::attackTarget, true);
	m_attackCollider->connect("area_entered", callable_mp(this, &AttackComponent::areaEnteredCollider));
	m_attackCollider->connect("area_exited", callable_mp(this, &AttackComponent::areaExitedCollider));
}

void AttackComponent::_exit_tree() {
	RETURN_IF_EDITOR(void())
}

void AttackComponent::areaEnteredCollider(godot::Area3D* area) {
	++m_numOfHitNodes;
	++m_numOfHitNodesTotal;

	if (auto* target = cast_to<RigidBody3D>(area->get_parent())) {
		Vector3 dir = Vector3(target->get_global_position() - get_global_position()).normalized();
		dir += Vector3(0, 1, 0);
		dir.normalize();
		target->set_linear_velocity(dir * 3.f);
		LOG(INFO, "area entered attack collider")
	}
}

void AttackComponent::areaExitedCollider(godot::Area3D* area) {
	--m_numOfHitNodes;
}

AttackComponent::EState AttackComponent::getAttackState() const {
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
