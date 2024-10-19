#include "parryinstigatorcomponent.h"

#include "parryInstance.h"
#include "parrytargetcomponent.h"

#include <godot_cpp/classes/character_body3d.hpp>
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
	ClassDB::bind_method(D_METHOD("getMass"), &ParryInstigatorComponent::getMass);
	ClassDB::bind_method(D_METHOD("setMass", "mass"), &ParryInstigatorComponent::setMass);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "Collision area"), "setPathToArea3D", "getPathToArea3D");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Mass"), "setMass", "getMass");
}

void ParryInstigatorComponent::_enter_tree() {
	RETURN_IF_EDITOR

	m_area = get_node<Area3D>(m_pathToArea3D);

	ASSERT_NOTNULL(m_area)

	m_area->connect("area_entered", callable_mp(this, &ParryInstigatorComponent::areaEnteredParryDetection));
	m_area->connect("area_exited", callable_mp(this, &ParryInstigatorComponent::areaExitedParryDetection));
}

void ParryInstigatorComponent::_exit_tree() {
	RETURN_IF_EDITOR

	ASSERT_NOTNULL(m_area)

	m_area->disconnect("area_entered", callable_mp(this, &ParryInstigatorComponent::areaEnteredParryDetection));
	m_area->disconnect("area_exited", callable_mp(this, &ParryInstigatorComponent::areaExitedParryDetection));
}

void ParryInstigatorComponent::_physics_process(double delta) { RETURN_IF_EDITOR }

void ParryInstigatorComponent::setPathToArea3D(NodePath path) { m_pathToArea3D = path; }

NodePath ParryInstigatorComponent::getPathToArea3D() const { return m_pathToArea3D; }

void ParryInstigatorComponent::setMass(float mass) { m_mass = mass; }

float ParryInstigatorComponent::getMass() const { return m_mass; }

void ParryInstigatorComponent::areaEnteredParryDetection(Area3D* area) {
	if (m_area->get_rid() == area->get_rid()) {
		LOG(DEBUG, "ParryInstigatorComponent cannot collide with itself",
				get_parent() ? get_parent()->get_name() : get_name())
		return;
	}
	if (auto* parrytarget = getAdjacentNode<ParryTargetComponent>(area)) {
		LOG(DEBUG, "ParryTarget entered area", area->get_parent()->get_name())
		m_inRangeParryTargets.emplace(area->get_rid().get_id(), *parrytarget);
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

	if (m_inRangeParryTargets.empty()) {
		LOG(DEBUG, "No in range parry targets");
		return;
	}

	// get closest in range parry target
	const Vector3 instigatorPosition = m_area->get_global_position();
	Vector3 closest = m_inRangeParryTargets.begin()->second.getPosition();
	ParryTargetComponent* target = &m_inRangeParryTargets.begin()->second;
	for (const auto& [rid, parryTarget] : m_inRangeParryTargets) {
		DebugDraw::Line(instigatorPosition, parryTarget.getPosition(), Color(1, 0, 0), 1.f);
		if (Vector3(instigatorPosition - parryTarget.getPosition()).length_squared() < closest.length_squared()) {
			closest = parryTarget.getPosition();
			target = &parryTarget;
		}
	}
	ASSERT_NOTNULL(target)
	ParryInstance instance(*this, *target);
	target->getParried(instance);
}

Vector3 ParryInstigatorComponent::getPosition() const {
	RETURN_IF_EDITOR_RET(Vector3)
	ASSERT_NOTNULL(m_area)
	return m_area->get_global_position();
}

Vector3 ParryInstigatorComponent::getVelocity() const {
	RETURN_IF_EDITOR_RET(Vector3)
	if (auto* characterBody = cast_to<CharacterBody3D>(get_parent())) { return characterBody->get_velocity(); }
	return {};
}

Vector3 ParryInstigatorComponent::getDesiredDirection() const {
	if (m_getDesiredDirectionCb) { return m_getDesiredDirectionCb(); }
	LOG(ERROR, "ParryInstigatorComponent does not have getDesiredDirectionCb set: ", get_parent()->get_name())
	return {};
}
