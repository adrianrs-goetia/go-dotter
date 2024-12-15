#pragma once

#include "typedefs.hpp"
#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <variant>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

class WrapperParryInstances;

class ParryTargetComponent : public NodeComponent {
	GDCLASS(ParryTargetComponent, NodeComponent)

public:
	// enum EParryTargetTag {
	// 	NONE = -1,
	// 	DYNAMIC_LIGHT = 0,
	// 	DYNAMIC_HEAVY = 1,
	// 	STATIC_OBJECT = 2,
	// 	STATIC_TERRAIN = 3,
	// };

public:
	godot::NodePath m_colliderPath;
	godot::Area3D* m_areaPtr = nullptr;

	std::function<void(const WrapperParryInstances& instance)> _callback = nullptr;

	// EParryTargetTag m_parryTag = EParryTargetTag::NONE;

	std::shared_ptr<ParryContact> m_parryContact;

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(ParryTargetComponent, ColliderPath, NODE_PATH)
		// BIND_ENUM_CONSTANT(DYNAMIC_LIGHT)
		// BIND_ENUM_CONSTANT(DYNAMIC_HEAVY)
		// BIND_ENUM_CONSTANT(STATIC_OBJECT)
		// BIND_ENUM_CONSTANT(STATIC_TERRAIN)
		// METHOD_PROPERTY_ENUM_IMPL(
		// 		ParryTargetComponent, ParryTag, INT, "DYNAMIC_LIGHT,DYNAMIC_HEAVY,STATIC_OBJECT,STATIC_TERRAIN")
	}

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() {
		RETURN_IF_EDITOR(void())

		set_name(get_class());

		m_areaPtr = get_node<godot::Area3D>(m_colliderPath);
		ASSERT_NOTNULL(m_areaPtr)
		m_areaPtr->set_collision_layer_value(collisionflags::parryTarget, true);
	}

	std::shared_ptr<ParryContact> onParried(const WrapperParryInstances& parryInstance) {
		if (_callback) {
			_callback(parryInstance);
		}
		m_parryContact = std::make_shared<ParryContact>([this]() { return this; });
		return m_parryContact;
	}

	void onAction(const WrapperParryInstances& freeze) {
		if (_callback) {
			_callback(freeze);
		}
	}

	godot::Vector3 getPosition() const {
		RETURN_IF_EDITOR(godot::Vector3())
		ASSERT_NOTNULL(m_areaPtr)
		return m_areaPtr->get_global_position();
	}

	GS_PATH_IMPL(m_colliderPath, ColliderPath)
	// GS_ENUM_IMPL(m_parryTag, ParryTag, EParryTargetTag)
};

// VARIANT_ENUM_CAST(ParryTargetComponent::EParryTargetTag)
