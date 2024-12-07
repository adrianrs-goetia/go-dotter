#include <components/grappleTargetComponent.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

using namespace godot;

void GrappleTargetComponent::_bind_methods() {
	METHOD_PROPERTY_IMPL(GrappleTargetComponent, AreaPath, NODE_PATH)
}

void GrappleTargetComponent::_enter_tree() {
	set_name(get_class());
	if (!getParentNode<RigidBody3D>(this) && !getParentNode<CharacterBody3D>(this) && !m_anchored) {
		m_anchored = true;
		LOG(WARN, "A Node that is not inheriting from either Rigidbody or CharacterBody3D must be an anchor");
	}

	RETURN_IF_EDITOR(void())
	if (m_pathToArea3D.is_empty()) {
		LOG(INFO, "Path to area3D is empty for", get_parent()->get_name())
		return;
	}
	m_area = get_node<Area3D>(m_pathToArea3D);
	if (!m_area) {
		String msg;
		msg += get_parent()->get_name();
		msg += ", failed to fetch m_areaPtr for collision. Please assign a path to a CollisionShape3D to 'Area Path'";
		ERR_PRINT_ONCE(msg);
	}
}

void GrappleTargetComponent::_exit_tree() {
	NodeComponent::_exit_tree();
	m_area = nullptr;
}

RID GrappleTargetComponent::getRid() const {
	if (!m_area || !is_inside_tree()) {
		return RID();
	}
	return m_area->get_rid();
}
