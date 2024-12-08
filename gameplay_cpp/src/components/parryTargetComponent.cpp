#include "parryTargetComponent.h"

#include <components/dataObjects/parryInstance.hpp>

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <debugdraw3d/api.h>

using namespace godot;

VARIANT_ENUM_CAST(ParryTargetComponent::EParryTargetTag)

void ParryTargetComponent::_bind_methods() {
	METHOD_PROPERTY_IMPL(ParryTargetComponent, ColliderPath, NODE_PATH)
	METHOD_PROPERTY_IMPL(ParryTargetComponent, Mass, FLOAT)

	BIND_ENUM_CONSTANT(DYNAMIC_LIGHT)
	BIND_ENUM_CONSTANT(DYNAMIC_HEAVY)
	BIND_ENUM_CONSTANT(STATIC_OBJECT)
	BIND_ENUM_CONSTANT(STATIC_TERRAIN)
	METHOD_PROPERTY_ENUM_IMPL(
			ParryTargetComponent, ParryTag, INT, "DYNAMIC_LIGHT,DYNAMIC_HEAVY,STATIC_OBJECT,STATIC_TERRAIN")
}

void ParryTargetComponent::_enter_tree() {
	RETURN_IF_EDITOR(void())
	
	set_name(get_class());
	m_areaPtr = get_node<Area3D>(m_colliderPath);
	if (!m_areaPtr) {
		String msg;
		msg += get_parent()->get_name();
		msg += ", failed to fetch m_areaPtr for collision. Please assign a path to a CollisionShape3D to 'Area Path'";
		ERR_PRINT_ONCE(msg);
	}
	else {
		m_areaPtr->set_collision_layer_value(collisionflags::parryTarget, true);
	}

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
	switch (getParryTagEnum()) {
		case EParryTargetTag::DYNAMIC_LIGHT: {
			if (auto* rigidBody = cast_to<RigidBody3D>(get_parent())) {
				// rigidBody->set_linear_velocity(parryInstance.getNewTargetVelocity());
				rigidBody->set_linear_velocity(Vector3(0, 3, 0));
			}
			else {
				LOG(DEBUG, "Parry DYNAMIC_LIGHT requires RigidBody3D: ", get_parent()->get_name())
			}
			break;
		}

		default:
			LOG(WARN, "Parried target without tag: ", getParryTag()) break;
	}

	if (m_onParriedCb) {
		m_onParriedCb();
	}
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
	// Vector3 dir = getVelocity() * (getIsHeavy() ? 1.f : -1.f);
	Vector3 dir = getVelocity();
	dir.y = Math::abs(dir.y);
	dir.normalize();
	return dir;
}
