#include "parrytargetcomponent.h"

#include "parryInstance.h"

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <debugdraw3d/api.h>

void ParryTargetComponent::_bind_methods() {
	DEFAULT_PROPERTY(ParryTargetComponent)
	//

	godot::ClassDB::bind_method(D_METHOD("getAreaPath"), &ParryTargetComponent::getAreaPath);
	godot::ClassDB::bind_method(D_METHOD("setAreaPath", "path"), &ParryTargetComponent::setAreaPath);
	godot::ClassDB::bind_method(D_METHOD("getIsHeavy"), &ParryTargetComponent::getIsHeavy);
	godot::ClassDB::bind_method(D_METHOD("setIsHeavy", "isHeavy"), &ParryTargetComponent::setIsHeavy);
	ClassDB::bind_method(D_METHOD("getMass"), &ParryTargetComponent::getMass);
	ClassDB::bind_method(D_METHOD("setMass", "mass"), &ParryTargetComponent::setMass);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "Collision AreaPath"), "setAreaPath", "getAreaPath");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "Is Heavy"), "setIsHeavy", "getIsHeavy");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Mass"), "setMass", "getMass");
}

void ParryTargetComponent::_enter_tree() {
	m_areaPtr = get_node<Area3D>(m_pathToArea3D);
	if (!m_areaPtr) {
		String msg;
		msg += get_parent()->get_name();
		msg += ", failed to fetch m_areaPtr for collision. Please assign a path to a CollisionShape3D to 'Area Path'";
		ERR_PRINT_ONCE(msg);
	}
	else { m_areaPtr->set_collision_layer_value(collisionflags::parryTarget, true); }

	RETURN_IF_EDITOR_RET(void)
	ASSERT_NOTNULL(m_areaPtr)
}

void ParryTargetComponent::_exit_tree() {
	if (m_areaPtr) { m_areaPtr->set_collision_layer_value(collisionflags::parryTarget, true); }
	m_areaPtr = nullptr;
}

void ParryTargetComponent::setAreaPath(NodePath path) { m_pathToArea3D = path; }

NodePath ParryTargetComponent::getAreaPath() const { return m_pathToArea3D; }

void ParryTargetComponent::setIsHeavy(bool isHeavy) { m_isHeavy = isHeavy; }

bool ParryTargetComponent::getIsHeavy() const { return m_isHeavy; }

void ParryTargetComponent::setMass(float mass) { m_mass = mass; }

float ParryTargetComponent::getMass() const { return m_mass; }

void ParryTargetComponent::setOnParriedCb(OnParriedCb&& cb) { m_onParriedCb = cb; }

void ParryTargetComponent::getParried(const ParryInstance& parryInstance) {
	if (auto* rigidBody = cast_to<RigidBody3D>(get_parent())) {
		rigidBody->set_linear_velocity(parryInstance.getNewTargetVelocity());
		DebugDraw::Line(parryInstance.targetPosition,
				parryInstance.targetPosition + (parryInstance.getNewTargetVelocity() * 2.f), Color(1.0, 0.3, 1.0), 1.f);
	}
	else { LOG(DEBUG, "Parrying static object: ", get_parent()->get_name()) }

	if (m_onParriedCb) m_onParriedCb();
}

Vector3 ParryTargetComponent::getPosition() const {
	RETURN_IF_EDITOR_RET(Vector3)
	ASSERT_NOTNULL(m_areaPtr)
	return m_areaPtr->get_global_position();
}

Vector3 ParryTargetComponent::getVelocity() const {
	RETURN_IF_EDITOR_RET(Vector3)

	if (auto* rigidBody = cast_to<RigidBody3D>(get_parent())) { return rigidBody->get_linear_velocity(); }
	return {};
}

Vector3 ParryTargetComponent::getDesiredDirection() const {
	Vector3 dir = getVelocity() * (getIsHeavy() ? 1.f : -1.f);
	dir.y = Math::abs(dir.y);
	dir.normalize();
	return dir;
}
