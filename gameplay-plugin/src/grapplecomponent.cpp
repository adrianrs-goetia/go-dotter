#include <components/grapplecomponent.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

void GrappleComponent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_anchor"), &GrappleComponent::get_is_anchor);
	ClassDB::bind_method(D_METHOD("set_is_anchor"), &GrappleComponent::set_is_anchor);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "anchored"), "set_is_anchor", "get_is_anchor");
}

void GrappleComponent::set_is_anchor(bool val) {
	if (get_parent_node<RigidBody3D>() || get_parent_node<CharacterBody3D>()) {
		is_anchor = val;
		return;
	}
	LOG(WARN, "A Node that is not inheriting from either Rigidbody or CharacterBody3D must be an anchor");
}

bool GrappleComponent::get_is_anchor() const { return is_anchor; }
