#include "parryInstigator.h"

#include "parryTarget.hpp"
#include <components/dataObjects/parryInstance.hpp>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/world3d.hpp>

#include <debugdraw3d/api.h>

using namespace godot;

void ComponentParryInstigator::_bind_methods() {
	METHOD_PROPERTY_IMPL(ComponentParryInstigator, ColliderPath, NODE_PATH)
}

void ComponentParryInstigator::_enter_tree() {
	set_name(get_class());
	RETURN_IF_EDITOR(void())

	m_area = get_node<Area3D>(m_colliderPath);

	ASSERT_NOTNULL(m_area)

	m_area->connect("area_entered", callable_mp(this, &ComponentParryInstigator::areaEnteredParryDetection));
	m_area->connect("area_exited", callable_mp(this, &ComponentParryInstigator::areaExitedParryDetection));
}

void ComponentParryInstigator::_exit_tree() {
	RETURN_IF_EDITOR(void())

	ASSERT_NOTNULL(m_area)

	m_area->disconnect("area_entered", callable_mp(this, &ComponentParryInstigator::areaEnteredParryDetection));
	m_area->disconnect("area_exited", callable_mp(this, &ComponentParryInstigator::areaExitedParryDetection));
}

void ComponentParryInstigator::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())
}

void ComponentParryInstigator::areaEnteredParryDetection(Area3D* area) {
	if (m_area->get_rid() == area->get_rid()) {
		LOG(DEBUG,
			"ComponentParryInstigator cannot collide with itself",
			get_parent() ? get_parent()->get_name() : get_name())
		return;
	}
	if (auto* parrytarget = getAdjacentNode<ComponentParryTarget>(area)) {
		// LOG(DEBUG, "ParryTarget entered area", area->get_parent()->get_name())
		m_inRangeParryTargets.emplace(area->get_rid().get_id(), *parrytarget);
	}
}

void ComponentParryInstigator::areaExitedParryDetection(Area3D* area) {
	auto it = m_inRangeParryTargets.find(area->get_rid().get_id());
	if (it != m_inRangeParryTargets.end()) {
		// LOG(DEBUG, "ParryTarget left area", area->get_parent()->get_name())
		m_inRangeParryTargets.erase(it);
	}
}

ComponentParryTarget* ComponentParryInstigator::getLastParryContactAssert() const {
	auto lock = m_lastParryContact.lock();
	ASSERT_NOTNULL(lock)
	return lock->getTarget();
}

std::optional<ParryInstance> ComponentParryInstigator::activateParry(ActivateParams params) {
	RETURN_IF_EDITOR(std::nullopt)
	ASSERT_NOTNULL(m_area)

	if (m_inRangeParryTargets.empty()) {
		LOG(DEBUG, "No in range parry targets");
		return std::nullopt;
	}

	// get closest in range parry target
	const Vector3 instigatorPosition = m_area->get_global_position();
	Vector3 closest = m_inRangeParryTargets.begin()->second.getPosition();
	ComponentParryTarget* target = &m_inRangeParryTargets.begin()->second;
	for (const auto& [rid, parryTarget] : m_inRangeParryTargets) {
		DebugDraw::Line(instigatorPosition, parryTarget.getPosition(), Color(1, 0, 0), 1.f);
		if (Vector3(instigatorPosition - parryTarget.getPosition()).length_squared() < closest.length_squared()) {
			closest = parryTarget.getPosition();
			target = &parryTarget;
		}
	}
	ASSERT_NOTNULL(target)

	ParryInstance instance(*this, *target, params);
	m_lastParryContact = target->onParried({ instance });

	return std::move(instance);
}

Vector3 ComponentParryInstigator::getPosition() const {
	RETURN_IF_EDITOR(Vector3())
	ASSERT_NOTNULL(m_area)
	return m_area->get_global_position();
}

Vector3 ComponentParryInstigator::getVelocity() const {
	RETURN_IF_EDITOR(Vector3())
	if (auto* characterBody = cast_to<CharacterBody3D>(get_parent())) {
		return characterBody->get_velocity();
	}
	return {};
}
