#include <components/parrycomponent.h>

#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/world3d.hpp>

#include <debugdraw3d/api.h>

void ParryComponent::_bind_methods() { DEFAULT_PROPERTY(ParryComponent) }

void ParryComponent::_enter_tree() {
	RETURN_IF_EDITOR

	m_area = get_child_node_of_type<Area3D>(this);
	ASSERT_NOTNULL(m_area)
	m_collisionshape = get_child_node_of_type<CollisionShape3D>(m_area);
	ASSERT_NOTNULL(m_collisionshape)
	m_rid_ignores.append(m_area->get_rid());
}

void ParryComponent::_physics_process(double delta) {
	RETURN_IF_EDITOR
	m_colliding_positions = _get_collision_query();
	// for (int i = 0; i < m_colliding_positions.size(); ++i) {
	// 	DebugDraw::Position(
	// 			Transform3D(Basis(Quaternion(), Vector3(2, 2, 2)), m_colliding_positions[i]), Color(0, 1, 0));
	// }
}

TypedArray<Vector3> ParryComponent::get_colliding_positions() const { return m_colliding_positions; }

Vector3 ParryComponent::get_closest_colliding_position() const {
	if (!m_colliding_positions.is_empty()) {
		Vector3 closest = m_colliding_positions[0];
		const Vector3 pos = get_global_position();
		for (int i = 0; i < m_colliding_positions.size(); i++) {
			const Vector3 v3 = m_colliding_positions[i];
			if (Vector3(pos - v3).length_squared() < Vector3(pos - closest).length_squared()) { closest = v3; }
		}
		return closest;
	}
	return Vector3();
}

TypedArray<Vector3> ParryComponent::_get_collision_query() const {
	PhysicsDirectSpaceState3D* space_state = get_viewport()->get_world_3d()->get_direct_space_state();
	ASSERT_NOTNULL(space_state)
	return space_state->collide_shape(_get_physicsshapequeryparams());
}

Ref<PhysicsShapeQueryParameters3D> ParryComponent::_get_physicsshapequeryparams() const {
	ASSERT_NOTNULL(m_area)
	ASSERT_NOTNULL(m_collisionshape)

	Ref<PhysicsShapeQueryParameters3D> query;
	query.instantiate();
	query->set_shape(m_collisionshape->get_shape());
	query->set_transform(get_global_transform());
	query->set_collide_with_areas(true);
	query->set_collide_with_bodies(true);
	query->set_exclude(m_rid_ignores);
	return query;
}