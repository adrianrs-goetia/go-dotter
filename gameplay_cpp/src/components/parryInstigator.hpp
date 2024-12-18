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

public:
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

		ASSERT_NOTNULL(m_area)

		m_area->connect("area_entered", callable_mp(this, &ComponentParryInstigator::areaEnteredParryDetection));
		m_area->connect("area_exited", callable_mp(this, &ComponentParryInstigator::areaExitedParryDetection));
	}

	void _exit_tree() override {
		RETURN_IF_EDITOR(void())

		ASSERT_NOTNULL(m_area)

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
		ASSERT_NOTNULL(lock)
		return lock->getTarget();
	}

	std::optional<EventParry> activateParry(EventParry::Params params) {
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

		EventParry instance{ getPosition(), target->getPosition(), std::move(params) };
		m_lastParryContact = target->onParried({ instance });

		return std::move(instance);
	}

	godot::Vector3 getPosition() const {
		RETURN_IF_EDITOR(Vector3())
		ASSERT_NOTNULL(m_area)
		return m_area->get_global_position();
	}

	godot::Vector3 getVelocity() const {
		RETURN_IF_EDITOR(Vector3())
		if (auto* characterBody = cast_to<CharacterBody3D>(get_parent())) {
			return characterBody->get_velocity();
		}
		return {};
	}
};
