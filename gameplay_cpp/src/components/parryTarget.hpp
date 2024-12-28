#pragma once

#include "typedefs.hpp"
#include <core/core.hpp>
#include <core/nodeComponent.hpp>
#include <events/parry.hpp>

#include <variant>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

class ComponentParryTarget : public NodeComponent {
	GDCLASS(ComponentParryTarget, NodeComponent)

public:
	// enum EParryTargetTag {
	// 	NONE = -1,
	// 	DYNAMIC_LIGHT = 0,
	// 	DYNAMIC_HEAVY = 1,
	// 	STATIC_OBJECT = 2,
	// 	STATIC_TERRAIN = 3,
	// };

	GS_PATH_IMPL(m_colliderPath, ColliderPath)
	// GS_ENUM_IMPL(m_parryTag, ParryTag, EParryTargetTag)

	godot::Area3D* m_areaPtr = nullptr;

public:
	std::function<void(const VEventParry& instance)> _callback = nullptr;

	// EParryTargetTag m_parryTag = EParryTargetTag::NONE;

	std::shared_ptr<ParryContact> m_parryContact;

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(ComponentParryTarget, ColliderPath, NODE_PATH)
		// BIND_ENUM_CONSTANT(DYNAMIC_LIGHT)
		// BIND_ENUM_CONSTANT(DYNAMIC_HEAVY)
		// BIND_ENUM_CONSTANT(STATIC_OBJECT)
		// BIND_ENUM_CONSTANT(STATIC_TERRAIN)
		// METHOD_PROPERTY_ENUM_IMPL(
		// 		ComponentParryTarget, ParryTag, INT, "DYNAMIC_LIGHT,DYNAMIC_HEAVY,STATIC_OBJECT,STATIC_TERRAIN")
	}

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() {
		RETURN_IF_EDITOR(void())

		set_name(get_class());

		m_areaPtr = get_node<godot::Area3D>(m_colliderPath);
		ASSERTNN(m_areaPtr)
		m_areaPtr->set_collision_layer_value(collisionflags::parryTarget, true);
	}

	std::shared_ptr<ParryContact> onParried(const VEventParry& parryInstance) {
		if (_callback) {
			_callback(parryInstance);
		}
		m_parryContact = std::make_shared<ParryContact>([this]() { return this; });
		return m_parryContact;
	}

	void onAction(const VEventParry& freeze) {
		if (_callback) {
			_callback(freeze);
		}
	}

	godot::Vector3 getPosition() const {
		RETURN_IF_EDITOR(godot::Vector3())
		ASSERTNN(m_areaPtr)
		return m_areaPtr->get_global_position();
	}

	godot::RID getRID() const {
		ASSERTNN(m_areaPtr)
		return m_areaPtr->get_rid();
	}
};

// VARIANT_ENUM_CAST(ComponentParryTarget::EParryTargetTag)
