#include <npcs/turret/turretnode.h>

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include <debugdraw3d/api.h>

static constexpr float FIRING_INTERVAL = 2.0f;
static constexpr float FIRING_INTERVAL_VARIANCE = 1.0f;
static constexpr float FIRING_STRENGTH = 24.f;

void TurretNode::_bind_methods() {
	DEFAULT_PROPERTY(TurretNode)
	//
}

void TurretNode::_enter_tree() {
	RETURN_IF_EDITOR
	//

	// TODO not hardcode get player. Have detection component?
	m_target = get_node<Node3D>(nodePaths::player);
	m_gunRotPoint = get_node<Node3D>(paths::gunRotPoint());
	m_gunRotJoint = get_node<Node3D>(paths::gunRotJoint());
	m_gunOpening = get_node<Node3D>(paths::gunOpening());
	// ResourceLoader should cache this between each load.
	// Still ugly way of getting the desired scene, only option I have seen so far is
	// having a string property to fetch the scene...
	m_projectileResource = ResourceLoader::get_singleton()->load("res://gameplayscenes/projectile.tscn");
	m_firingTimer = memnew(Timer);

	ASSERT(m_projectileResource.is_valid(), "");
	ASSERT_NOTNULL(m_target)
	ASSERT_NOTNULL(m_gunRotPoint)
	ASSERT_NOTNULL(m_gunRotJoint)
	ASSERT_NOTNULL(m_gunOpening)
	ASSERT_NOTNULL(m_firingTimer)

	add_child(m_firingTimer);
	m_firingTimer->connect("timeout", callable_mp(this, &TurretNode::fireProjectile));
	m_firingTimer->start(getFiringInterval());
}

void TurretNode::_physics_process(double delta) {
	RETURN_IF_EDITOR
	//

	rotateHeadTowardsTarget();
}

float TurretNode::getFiringInterval() const {
	return FIRING_INTERVAL + canonicalRandomNumber(-FIRING_INTERVAL_VARIANCE, FIRING_INTERVAL_VARIANCE);
}

void TurretNode::rotateHeadTowardsTarget() {
	ASSERT_NOTNULL(m_gunRotPoint)
	ASSERT_NOTNULL(m_gunRotJoint)
	/**
	 * These rotations are highly dependent on transform of the child pieces in the model
	 */
	// Yaw rotation
	{
		const Vector3 toTarget3D = getDirectionToTarget(m_gunRotPoint);
		const Vector3 toTarget = Vector3(toTarget3D.x, 0, toTarget3D.z).normalized();
		Basis basis = m_gunRotPoint->get_global_basis();
		basis.set_column(0, g_up.cross(toTarget) * basis.get_column(0).length()); // x
		basis.set_column(1, g_up * basis.get_column(1).length()); // y
		basis.set_column(2, toTarget * basis.get_column(2).length()); // z
		m_gunRotPoint->set_global_basis(basis);
	}
	// Pitch rotation
	{
		const Vector3 toTarget = getDirectionToTarget(m_gunRotJoint);
		const float pitch = g_up.dot(toTarget) * -1.f; // Model requires negative pitch here
		const Vector3 currentRotation = m_gunRotJoint->get_rotation();
		m_gunRotJoint->set_rotation(Vector3(asinf(pitch), currentRotation.y, currentRotation.z));
	}
}

void TurretNode::fireProjectile() {
	Node* instance = m_projectileResource->instantiate();
	ASSERT_NOTNULL(instance)

	Node* root = get_node<Node>(nodePaths::root);
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