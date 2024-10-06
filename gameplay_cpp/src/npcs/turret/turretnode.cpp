#include <npcs/turret/turretnode.h>

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include <debugdraw3d/api.h>

static constexpr float FIRING_INTERVAL = 2.5f;
static constexpr float FIRING_INTERVAL_VARIANCE = 0.4f;
static constexpr float FIRING_STRENGTH = 20.f;

void TurretNode::_bind_methods() {
	DEFAULT_PROPERTY(TurretNode)
	//
}

void TurretNode::_enter_tree() {
	RETURN_IF_EDITOR

	// TODO not hardcode get player. have detection system?
	m_target = get_node<Node3D>(NodePaths::player);
	m_gunRotPoint = get_node<Node3D>(paths::gunRotPoint().c_str());
	m_gunRotJoint = get_node<Node3D>(paths::gunRotJoint().c_str());
	m_gunOpening = get_node<Node3D>(paths::gunOpening().c_str());
	m_projectileResource = ResourceLoader::get_singleton()->load("res://gameplayscenes/projectile.tscn");
	m_firingTimer = memnew(Timer);

	ASSERT(m_projectileResource.is_valid(), "");
	ASSERT_NOTNULL(m_target)
	ASSERT_NOTNULL(m_gunRotPoint)
	ASSERT_NOTNULL(m_gunRotJoint)
	ASSERT_NOTNULL(m_gunOpening)
	ASSERT_NOTNULL(m_firingTimer)

	add_child(m_firingTimer);
	m_firingTimer->connect("timeout", callable_mp(this, &TurretNode::fire_projectile));
	m_firingTimer->start(getFiringInterval());
}

void TurretNode::_physics_process(double delta) {
	RETURN_IF_EDITOR

	//
	rotate_head_towards_target();
}

float TurretNode::getFiringInterval() const {
	return FIRING_INTERVAL + canonical_random_number(-FIRING_INTERVAL_VARIANCE, FIRING_INTERVAL_VARIANCE);
}

void TurretNode::rotate_head_towards_target() {
	ASSERT_NOTNULL(m_gunRotPoint)
	ASSERT_NOTNULL(m_gunRotJoint)
	// Yaw rotation
	{
		const Vector3 toTarget3D = getDirectionToTarget(m_gunRotPoint);
		const Vector2 toTarget(toTarget3D.z, toTarget3D.x);
		const float yaw = toTarget.angle();
		const Vector3 currentRotation = m_gunRotPoint->get_rotation();
		m_gunRotPoint->set_rotation(Vector3(currentRotation.x, yaw, currentRotation.z));
	}
	// Pitch rotation
	{
		const Vector3 toTarget = getDirectionToTarget(m_gunRotJoint);
		const float pitch = g_up.dot(toTarget) * -1.f; // Model requires negative pitch here
		const Vector3 currentRotation = m_gunRotJoint->get_rotation();
		m_gunRotJoint->set_rotation(Vector3(asinf(pitch), currentRotation.y, currentRotation.z));
	}
}

void TurretNode::fire_projectile() {
	Node* instance = m_projectileResource->instantiate();
	ASSERT_NOTNULL(instance)

	Node* root = get_node<Node>(NodePaths::root);
	ASSERT_NOTNULL(root)
	root->add_child(instance);

	if (auto* rigidbody = cast_to<RigidBody3D>(instance)) {
		rigidbody->set_global_position(getGunOpeningLocation());
		rigidbody->set_linear_velocity(getGunOpeningDirection() * FIRING_STRENGTH);
	}
	m_firingTimer->start(getFiringInterval());
}

Vector3 TurretNode::getDirectionToTarget(const Node3D* source) const {
	return Vector3((m_target->get_global_position() + Vector3(0, /*@ todo get player centre*/ 1, 0)) -
			source->get_global_position())
			.normalized();
}

Vector3 TurretNode::getGunOpeningLocation() const {
	return m_gunOpening->get_global_position() + (getGunOpeningDirection() * 1.f);
}

Vector3 TurretNode::getGunOpeningDirection() const {
	return m_gunOpening->get_global_basis().get_column(2).normalized(); // Positive Z forward
}