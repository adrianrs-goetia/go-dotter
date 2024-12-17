#include "attackComponent.h"

#include "attackTargetComponent.h"
#include <events/attack.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <debugdraw3d/api.h>

using namespace godot;

void ComponentAttackInstigator::_bind_methods() {
	METHOD_PROPERTY_IMPL(ComponentAttackInstigator, ColliderPath, NODE_PATH)
	METHOD_INOUT_BIND(ComponentAttackInstigator, areaEnteredCollider, areaExitedCollider, area)
}

void ComponentAttackInstigator::setComponentEnabled(bool enabled) {
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

void ComponentAttackInstigator::_enter_tree() {
	RETURN_IF_EDITOR(void())

	m_attackCollider = get_node<Area3D>(m_attackColliderPath);
	ASSERT_NOTNULL(m_attackCollider)

	m_attackCollider->set_collision_mask_value(collisionflags::attackTarget, true);
	m_attackCollider->connect("area_entered", callable_mp(this, &ComponentAttackInstigator::areaEnteredCollider));
	m_attackCollider->connect("area_exited", callable_mp(this, &ComponentAttackInstigator::areaExitedCollider));

	setComponentEnabled(false);
}

void ComponentAttackInstigator::_exit_tree() {
	RETURN_IF_EDITOR(void())
}

void ComponentAttackInstigator::areaEnteredCollider(godot::Area3D* area) {
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

	// target->notification(ENotifications::ATTACKED);
	// auto g = target->get_groups();

	// if (auto* rb = cast_to<RigidBody3D>(target)) {
	// 	if (!rb->is_in_group(godotgroups::projectile)) {
	// 		Vector3 dir = Vector3(rb->get_global_position() - get_global_position()).normalized();
	// 		dir += Vector3(0, 1, 0);
	// 		dir.normalize();
	// 		dir *= 10.f;
	// 		rb->set_linear_velocity(dir);
	// 		DebugDraw::Line(rb->get_global_position(), rb->get_global_position() + dir, Color(1, 0, 0), 1.f);
	// 	}
	// }
}

void ComponentAttackInstigator::areaExitedCollider(godot::Area3D* area) {
	--m_numOfHitNodes;
}

ComponentAttackInstigator::EState ComponentAttackInstigator::getAttackState() const {
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
