#pragma once

#include "parryTarget.hpp"
#include "typedefs.hpp"

#include <core/core.hpp>
#include <core/nodeComponent.hpp>
#include <events/parry.hpp>

#include <functional>
#include <map>
#include <optional>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>

#include <debugdraw3d/api.h>

class ComponentParryInstigator : public NodeComponent {
	GDCLASS(ComponentParryInstigator, NodeComponent)

public:
	using GodotRID = uint64_t;

	GS_PATH_IMPL(m_colliderPath, ColliderPath)

	godot::Area3D* m_area = nullptr;
	std::map<GodotRID, ComponentParryTarget&> m_inRangeParryTargets;

	std::weak_ptr<ParryContact> m_lastParryContact;

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(ComponentParryInstigator, ColliderPath, NODE_PATH)
	}

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() override {
		set_name(get_class());
		RETURN_IF_EDITOR(void())

		m_area = get_node<Area3D>(m_colliderPath);

		ASSERTNN(m_area)

		m_area->connect("area_entered", callable_mp(this, &ComponentParryInstigator::areaEnteredParryDetection));
		m_area->connect("area_exited", callable_mp(this, &ComponentParryInstigator::areaExitedParryDetection));
	}

	void _exit_tree() override {
		RETURN_IF_EDITOR(void())

		ASSERTNN(m_area)

		m_area->disconnect("area_entered", callable_mp(this, &ComponentParryInstigator::areaEnteredParryDetection));
		m_area->disconnect("area_exited", callable_mp(this, &ComponentParryInstigator::areaExitedParryDetection));
	}

	void areaEnteredParryDetection(godot::Area3D* area) {
		if (m_area->get_rid() == area->get_rid()) {
			return;
		}
		if (auto* parrytarget = getAdjacentNode<ComponentParryTarget>(area)) {
			m_inRangeParryTargets.emplace(area->get_rid().get_id(), *parrytarget);
		}
	}

	void areaExitedParryDetection(godot::Area3D* area) {
		auto it = m_inRangeParryTargets.find(area->get_rid().get_id());
		if (it != m_inRangeParryTargets.end()) {
			m_inRangeParryTargets.erase(it);
		}
	}

	ComponentParryTarget* getLastParryContactAssert() const {
		auto lock = m_lastParryContact.lock();
		ASSERTNN(lock)
		return lock->getTarget();
	}

	ComponentParryTarget* getLastParryContact() const {
		auto lock = m_lastParryContact.lock();
		return lock ? lock->getTarget() : nullptr;
	}

	std::optional<EventParry> activateParry(EventParry::Params params) {
		RETURN_IF_EDITOR(std::nullopt)
		ASSERTNN(m_area)

		if (m_inRangeParryTargets.empty()) {
			return std::nullopt;
		}

		// get closest in range parry target
		const Vector3 instigatorPosition = m_area->get_global_position();
		Vector3 closest = m_inRangeParryTargets.begin()->second.getPosition();
		ComponentParryTarget* target = &m_inRangeParryTargets.begin()->second;
		for (const auto& [rid, parryTarget] : m_inRangeParryTargets) {
			if (Vector3(instigatorPosition - parryTarget.getPosition()).length_squared() < closest.length_squared()) {
				closest = parryTarget.getPosition();
				target = &parryTarget;
			}
		}
		ASSERTNN(target)

		EventParry instance{ getPosition(), target->getPosition(), std::move(params) };
		m_lastParryContact = target->onParried({ instance });

		return std::move(instance);
	}

	godot::Vector3 getPosition() const {
		RETURN_IF_EDITOR(Vector3())
		ASSERTNN(m_area)
		return m_area->get_global_position();
	}

	godot::Vector3 getVelocity() const {
		RETURN_IF_EDITOR(Vector3())
		if (auto* characterBody = cast_to<godot::CharacterBody3D>(get_parent())) {
			return characterBody->get_velocity();
		}
		return {};
	}

	godot::Vector3 getLastParryTargetDir() const {
		auto* target = getLastParryContactAssert();
		return godot::Vector3(target->get_global_position() - get_global_position()).normalized();
	}

	godot::Vector3 getLastParryTargetDir2D() const {
		auto* target = getLastParryContactAssert();
		const godot::Vector3 dir(target->get_global_position() - get_global_position());
		return godot::Vector3(dir.x, 0.0, dir.z).normalized();
	}
};
