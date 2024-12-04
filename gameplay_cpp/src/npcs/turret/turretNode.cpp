#include <npcs/turret/turretNode.h>

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include <configHandler.h>
#include <debugdraw3d/api.h>

using namespace godot;

void TurretNode::_bind_methods() {}

void TurretNode::_enter_tree() {
	m_gunOpening = get_node<Node3D>(paths::gunOpening());
	m_gunOpeningAudioPlayer = getChildOfNode<AudioStreamPlayer3D>(this);
	m_gunOpeningGpuParticles = getChildOfNode<GPUParticles3D>(this);

	RETURN_IF_EDITOR(void())
	//

	// TODO not hardcode get player. Have detection component?
	m_target = get_node<Node3D>(nodePaths::player);
	m_gunRotPoint = get_node<Node3D>(paths::gunRotPoint());
	m_gunRotJoint = get_node<Node3D>(paths::gunRotJoint());
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
	ASSERT_NOTNULL(m_gunOpeningAudioPlayer)

	add_child(m_firingTimer);
	m_firingTimer->connect("timeout", callable_mp(this, &TurretNode::fireProjectile));
	m_firingTimer->start(getFiringInterval());
}

void TurretNode::_physics_process(double delta) {
	if (m_gunOpening) {
		const Vector3 direction = getGunOpeningDirection() * -1.f;
		Transform3D gunOpeningAudioVisualTransform(
				createBasisFromDirection(direction), getGunOpeningLocation() + (direction * 0.5));
		if (m_gunOpeningAudioPlayer)
			m_gunOpeningAudioPlayer->set_global_transform(gunOpeningAudioVisualTransform);
		if (m_gunOpeningGpuParticles)
			m_gunOpeningGpuParticles->set_global_transform(gunOpeningAudioVisualTransform);
	}

	RETURN_IF_EDITOR(void())
	//

	m_isEnabled = GETPARAM_B("npcs", "turret", "enabled");

	rotateHeadTowardsTarget();
}

float TurretNode::getFiringInterval() const {
	const auto variance = GETPARAM_D("npcs", "turret", "firingIntervalVariance");
	return GETPARAM_D("npcs", "turret", "firingInterval") + canonicalRandomNumber(-variance, variance);
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
		m_gunRotPoint->set_global_basis(
				createBasisFromDirection(toTarget, getScaleFromBasis(m_gunRotPoint->get_global_basis())));
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
	if (!m_isEnabled) {
		return;
	}
	Node* instance = m_projectileResource->instantiate();
	ASSERT_NOTNULL(instance)

	Node* root = get_node<Node>(nodePaths::root);
	ASSERT_NOTNULL(root)
	root->add_child(instance);

	if (auto* rigidbody = cast_to<RigidBody3D>(instance)) {
		rigidbody->set_global_position(getGunOpeningLocation());
		rigidbody->set_linear_velocity(getGunOpeningDirection() * GETPARAM_D("npcs", "turret", "firingStrength"));
	}
	m_firingTimer->start(getFiringInterval());

	m_gunOpeningAudioPlayer->play();
	m_gunOpeningGpuParticles->restart();
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