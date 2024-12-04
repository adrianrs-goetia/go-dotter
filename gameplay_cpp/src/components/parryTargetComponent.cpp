#include "parryTargetComponent.h"

#include <components/dataObjects/parryInstance.hpp>

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <debugdraw3d/api.h>

using namespace godot;

void ParryTargetComponent::_bind_methods() {
	METHOD_PROPERTY_IMPL(ParryTargetComponent, AreaPath, NODE_PATH, "Collision AreaPath")
	METHOD_PROPERTY_IMPL(ParryTargetComponent, IsHeavy, BOOL, "Is heavy")
	METHOD_PROPERTY_IMPL(ParryTargetComponent, Mass, FLOAT, "MASS")
}

void ParryTargetComponent::_enter_tree() {
	set_name(get_class());
	m_areaPtr = get_node<Area3D>(m_pathToArea3D);
	if (!m_areaPtr) {
		String msg;
		msg += get_parent()->get_name();
		msg += ", failed to fetch m_areaPtr for collision. Please assign a path to a CollisionShape3D to 'Area Path'";
		ERR_PRINT_ONCE(msg);
	}
	else {
		m_areaPtr->set_collision_layer_value(collisionflags::parryTarget, true);
	}

	RETURN_IF_EDITOR(void())
	ASSERT_NOTNULL(m_areaPtr)
}

void ParryTargetComponent::_exit_tree() {
	if (m_areaPtr) {
		m_areaPtr->set_collision_layer_value(collisionflags::parryTarget, true);
	}
	m_areaPtr = nullptr;
}

void ParryTargetComponent::setOnParriedCb(OnParriedCb&& cb) {
	m_onParriedCb = cb;
}

void ParryTargetComponent::getParried(const ParryInstance& parryInstance) {
	if (auto* rigidBody = cast_to<RigidBody3D>(get_parent())) {
		rigidBody->set_linear_velocity(parryInstance.getNewTargetVelocity());
		DebugDraw::Line(parryInstance.targetPosition,
				parryInstance.targetPosition + (parryInstance.getNewTargetVelocity() * 2.f), Color(1.0, 0.3, 1.0), 1.f);
	}
	else {
		LOG(DEBUG, "Parrying static object: ", get_parent()->get_name())
	}

	if (m_onParriedCb)
		m_onParriedCb();
}

Vector3 ParryTargetComponent::getPosition() const {
	RETURN_IF_EDITOR(Vector3())
	ASSERT_NOTNULL(m_areaPtr)
	return m_areaPtr->get_global_position();
}

Vector3 ParryTargetComponent::getVelocity() const {
	RETURN_IF_EDITOR(Vector3())

	if (auto* rigidBody = cast_to<RigidBody3D>(get_parent())) {
		return rigidBody->get_linear_velocity();
	}
	return {};
}

Vector3 ParryTargetComponent::getDesiredDirection() const {
	Vector3 dir = getVelocity() * (getIsHeavy() ? 1.f : -1.f);
	dir.y = Math::abs(dir.y);
	dir.normalize();
	return dir;
}
