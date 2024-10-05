#include <npcs/turret/turretnode.h>

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <debugdraw3d/api.h>

static constexpr float TURRETHEAD_ROTATION_SPEED = 0.8f;
static constexpr float FIRING_INTERVAL = 1.5f;
static constexpr float FIRING_STRENGTH = 20.f;

void TurretNode::_bind_methods() {
	DEFAULT_PROPERTY(TurretNode)
	//
}

void TurretNode::_enter_tree() {
	RETURN_IF_EDITOR

	// TODO not hardcode get player. have detection system
	m_target = get_node<Node3D>(NodePaths::player);
	m_turretHead = get_node<Node3D>("TurretHead");
	m_projectileResource = ResourceLoader::get_singleton()->load("res://gameplayscenes/projectile.tscn");

	ASSERT(m_projectileResource.is_valid(), "");
	ASSERT_NOTNULL(m_target)
	ASSERT_NOTNULL(m_turretHead)
}

void TurretNode::_physics_process(double delta) {
	RETURN_IF_EDITOR

	//
	rotate_head_towards_target(delta * TURRETHEAD_ROTATION_SPEED);

	m_firingTimer += delta;
	if (m_firingTimer >= FIRING_INTERVAL) {
		m_firingTimer = 0.f;
		fire_projectile();
	}
}

void TurretNode::rotate_head_towards_target(float rotationSpeed) {
	// get angle of vector; turrethead -> target
	const Vector3 toTarget = getDirectionToTarget();

	const Vector3 crossRight = g_up.cross(toTarget);
	const Vector3 crossUp = toTarget.cross(crossRight);
	m_turretHead->set_basis(Basis(crossRight, crossUp, toTarget));

	const Vector3 pos = m_turretHead->get_global_position();
}

void TurretNode::fire_projectile() {
	ASSERT(m_projectileResource.is_valid(), "");
	Node* instance = m_projectileResource->instantiate();
	ASSERT_NOTNULL(instance)

	if (auto* rigidbody = cast_to<RigidBody3D>(instance)) {
		add_child(rigidbody);

		const Vector3 toTarget = getDirectionToTarget();
		rigidbody->set_global_position(m_turretHead->get_global_position() + (toTarget * 0.9f));
		const float scale = 0.1;
		rigidbody->set_scale(Vector3(scale, scale, scale));
		rigidbody->set_linear_velocity(toTarget * FIRING_STRENGTH);
	}
}

Vector3 TurretNode::getDirectionToTarget() const {
	// @todo get target centre
	return Vector3((m_target->get_global_position() + Vector3(0, 1, 0)) - m_turretHead->get_global_position())
			.normalized();
}
