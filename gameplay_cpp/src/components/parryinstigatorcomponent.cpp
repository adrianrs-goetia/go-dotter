#include "parryinstigatorcomponent.h"

#include "parrytargetcomponent.h"

#include <godot_cpp/classes/physics_direct_body_state3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/world3d.hpp>

#include <debugdraw3d/api.h>

using namespace godot;

void ParryInstigatorComponent::_bind_methods() {
	DEFAULT_PROPERTY(ParryInstigatorComponent)

	ClassDB::bind_method(D_METHOD("getPathToArea3D"), &ParryInstigatorComponent::getPathToArea3D);
	ClassDB::bind_method(D_METHOD("setPathToArea3D", "path"), &ParryInstigatorComponent::setPathToArea3D);
	ClassDB::bind_method(D_METHOD("areaEnteredParryDetection"), &ParryInstigatorComponent::areaEnteredParryDetection);
	ClassDB::bind_method(D_METHOD("areaExitedParryDetection"), &ParryInstigatorComponent::areaExitedParryDetection);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "Collision area"), "setPathToArea3D", "getPathToArea3D");
}

void ParryInstigatorComponent::_enter_tree() {
	RETURN_IF_EDITOR

	m_area = get_node<Area3D>(m_pathToArea3D);

	ASSERT_NOTNULL(m_area)

	m_area->connect("area_entered", callable_mp(this, &ParryInstigatorComponent::areaEnteredParryDetection));
	m_area->connect("area_exited", callable_mp(this, &ParryInstigatorComponent::areaExitedParryDetection));

	// m_area = getChildOfNode<Area3D>(this);
	// m_collisionShape = getChildOfNode<CollisionShape3D>(m_area);
	// ASSERT_NOTNULL(m_collisionShape)
	// m_ridIgnores.append(m_area->get_rid());
}

void ParryInstigatorComponent::_exit_tree() {
	RETURN_IF_EDITOR

	ASSERT_NOTNULL(m_area)

	m_area->disconnect("area_entered", callable_mp(this, &ParryInstigatorComponent::areaEnteredParryDetection));
	m_area->disconnect("area_exited", callable_mp(this, &ParryInstigatorComponent::areaExitedParryDetection));
}

void ParryInstigatorComponent::_physics_process(double delta) {
	RETURN_IF_EDITOR
	// m_collidingPositions = _getCollisionQuery();
	// for (int i = 0; i < m_collidingPositions.size(); ++i) {
	// 	DebugDraw::Position(
	// 			Transform3D(Basis(Quaternion(), Vector3(2, 2, 2)), m_collidingPositions[i]), Color(0, 1, 0));
	// }
}

void ParryInstigatorComponent::setPathToArea3D(NodePath path) { m_pathToArea3D = path; }

NodePath ParryInstigatorComponent::getPathToArea3D() { return m_pathToArea3D; }

void ParryInstigatorComponent::areaEnteredParryDetection(Area3D* area) {
	if (m_area->get_rid() == area->get_rid()) {
		LOG(DEBUG, "ParryInstigatorComponent cannot collide with itself",
				get_parent() ? get_parent()->get_name() : get_name())
		return;
	}
	if (auto* parrytarget = getAdjacentNode<ParryTargetComponent>(area)) {
		LOG(DEBUG, "ParryTarget entered area", area->get_parent()->get_name())
		m_inRangeParryTargets.emplace(area->get_rid().get_id(), parrytarget);
	}
}

void ParryInstigatorComponent::areaExitedParryDetection(Area3D* area) {
	auto it = m_inRangeParryTargets.find(area->get_rid().get_id());
	if (it != m_inRangeParryTargets.end()) {
		LOG(DEBUG, "ParryTarget left area", area->get_parent()->get_name())
		m_inRangeParryTargets.erase(it);
	}
}

void ParryInstigatorComponent::activateParry() {
	RETURN_IF_EDITOR
	ASSERT_NOTNULL(m_area)

	const Vector3 instigatorPosition = m_area->get_global_position();
	for(const auto& [rid, parryTarget] : m_inRangeParryTargets){
		DebugDraw::Line(instigatorPosition, parryTarget->getPosition(), Color(1,0,0), 1.f);
	}
}

// TypedArray<Vector3> ParryInstigatorComponent::getCollidingPositions() const { return m_collidingPositions; }

// Vector3 ParryInstigatorComponent::getClosestCollidingPosition() const {
// 	if (!m_collidingPositions.is_empty()) {
// 		Vector3 closest = m_collidingPositions[0];
// 		const Vector3 pos = get_global_position();
// 		for (int i = 0; i < m_collidingPositions.size(); i++) {
// 			const Vector3 v3 = m_collidingPositions[i];
// 			if (Vector3(pos - v3).length_squared() < Vector3(pos - closest).length_squared()) { closest = v3; }
// 		}
// 		return closest;
// 	}
// 	return Vector3();
// }

// TypedArray<Vector3> ParryInstigatorComponent::_getCollisionQuery() const {
// 	PhysicsDirectSpaceState3D* space_state = get_viewport()->get_world_3d()->get_direct_space_state();
// 	ASSERT_NOTNULL(space_state)
// 	return space_state->collide_shape(_get_physicsshapequeryparams());
// }

// Ref<PhysicsShapeQueryParameters3D> ParryInstigatorComponent::_get_physicsshapequeryparams() const {
// 	ASSERT_NOTNULL(m_area)
// 	ASSERT_NOTNULL(m_collisionShape)

// 	Ref<PhysicsShapeQueryParameters3D> query;
// 	query.instantiate();
// 	query->set_shape(m_collisionShape->get_shape());
// 	query->set_transform(get_global_transform());
// 	query->set_collide_with_areas(true);
// 	query->set_collide_with_bodies(true);
// 	query->set_exclude(m_ridIgnores);
// 	return query;
// }
