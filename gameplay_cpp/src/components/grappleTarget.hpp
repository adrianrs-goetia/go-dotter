#pragma once

#include "baseClasses/grappleBase.hpp"

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

class ComponentGrappleTarget : public ComponentGrappleBase {
	GDCLASS(ComponentGrappleTarget, ComponentGrappleBase)

private:
	GS_PATH_IMPL(m_colliderPath, ColliderPath)

	godot::Area3D* m_area = nullptr;

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(ComponentGrappleTarget, ColliderPath, NODE_PATH)
	}

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() override {
		set_name(get_class());
		if (!getParentNode<godot::RigidBody3D>(this) && !getParentNode<godot::CharacterBody3D>(this) && !m_anchored) {
			m_anchored = true;
		}

		RETURN_IF_EDITOR(void())
		if (m_colliderPath.is_empty()) {
			return;
		}
		m_area = get_node<godot::Area3D>(m_colliderPath);
	}

	void _exit_tree() override {
		NodeComponent::_exit_tree();
		m_area = nullptr;
	}

	godot::RID getRid() const {
		if (!m_area || !is_inside_tree()) {
			return RID();
		}
		return m_area->get_rid();
	}
};
