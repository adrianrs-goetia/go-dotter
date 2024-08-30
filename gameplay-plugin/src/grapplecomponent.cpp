#include <components/grapplecomponent.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

void GrappleComponent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_anchor"), &GrappleComponent::get_is_anchor);
	ClassDB::bind_method(D_METHOD("set_is_anchor"), &GrappleComponent::set_is_anchor);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "anchored"), "set_is_anchor", "get_is_anchor");
}

void GrappleComponent::_enter_tree() {
	if (!m_area) {
		TypedArray<Node> children = get_children();
		for (int i = 0; i < children.size(); ++i) {
			if (auto* a = cast_to<Area3D>(children[i])) {
				m_area = a;
				break;
			}
		}
	}
	if (m_area && !m_shape) {
		TypedArray<Node> children = m_area->get_children();
		for (int i = 0; i < children.size(); ++i) {
			if (auto* s = cast_to<CollisionShape3D>(children[i])) {
				m_shape = s;
				break;
			}
		}
	}
	if (!m_area) { LOG(DEBUG, "Grapplecomponent missing area,", get_name()) }
	if (!m_shape) { LOG(DEBUG, "Grapplecomponent missing shape,", get_name()) }
}

void GrappleComponent::_exit_tree() {
	m_area = nullptr;
	m_shape = nullptr;
}

RID GrappleComponent::get_rid() const {
	if (!m_area) {
		LOG(WARN, "GrappleComponent missing area, returning dummy id; name:", get_name())
		return RID();
	}
	return m_area->get_rid();
}

void GrappleComponent::set_is_anchor(bool val) {
	if (get_parent_node<RigidBody3D>() || get_parent_node<CharacterBody3D>()) {
		m_is_anchor = val;
		return;
	}
	LOG(WARN, "A Node that is not inheriting from either Rigidbody or CharacterBody3D must be an anchor");
}

bool GrappleComponent::get_is_anchor() const { return m_is_anchor; }
