#include <components/grapplecomponent.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <debugdraw3d/api.h>

void GrappleComponent::launch(GrappleComponent* subject) {
	switch (_determine_launch_type(subject)) {
		case LaunchType::BOTH_ANCHOR: {
			LOG(WARN, "GrappleComponent launch between two anchors. This shouldn't happen")
			return;
		}
		case LaunchType::BOTH_NON_ANCHOR: {
			const float max_mass = get_mass() + subject->get_mass();
			const float instigator_weight = get_mass() / max_mass;
			const float subject_weight = subject->get_mass() / max_mass;
			_impulse_owner(_determine_launch_direction_atob(this, subject), instigator_weight);
			subject->_impulse_owner(_determine_launch_direction_atob(subject, this), subject_weight);
			LOG(DEBUG, "Grapple launch -- BOTH_NON_ANCHOR")
		}
		case LaunchType::INSTIGATOR_ANCHOR: {
			subject->_impulse_owner(_determine_launch_direction_atob(subject, this));
			LOG(DEBUG, "Grapple launch -- INSTIGATOR_ANCHOR")
		}
		case LaunchType::SUBJECT_ANCHOR: {
			_impulse_owner(_determine_launch_direction_atob(this, subject));
			LOG(DEBUG, "Grapple launch -- SUBJECT_ANCHOR")
		}
	}
}

GrappleComponent::LaunchType GrappleComponent::_determine_launch_type(const GrappleComponent* subject) {
	if (get_is_anchor() && subject->get_is_anchor()) { return LaunchType::BOTH_ANCHOR; }
	else if (!get_is_anchor() && subject->get_is_anchor()) { return LaunchType::SUBJECT_ANCHOR; }
	else if (get_is_anchor() && !subject->get_is_anchor()) { return LaunchType::INSTIGATOR_ANCHOR; }
	return LaunchType::BOTH_NON_ANCHOR;
}

Vector3 GrappleComponent::_determine_launch_direction_atob(const GrappleComponent* a, const GrappleComponent* b) {
	return Vector3(b->get_global_position() - a->get_global_position()).normalized();
}

void GrappleComponent::_impulse_owner(const Vector3& direction, float launchstrengthmultiplier) {
	DebugDraw::Line(get_global_position(), get_global_position() + (direction * get_launch_strength()), Color(1,0,1), 1.f);
	if (auto* p = get_parent_node<RigidBody3D>()) {
		p->set_linear_velocity(direction * get_launch_strength() * launchstrengthmultiplier);
	}
	if (auto* p = get_parent_node<CharacterBody3D>()) {
		p->set_velocity(direction * m_launch_strength * launchstrengthmultiplier);
	}
	else {
		LOG(ERROR, "GrappleComponent impulse on invalid parent",
				get_parent()->get_name().get_basename().utf8().get_data())
	}
}

void GrappleComponent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_anchor"), &GrappleComponent::get_is_anchor);
	ClassDB::bind_method(D_METHOD("set_is_anchor", "val"), &GrappleComponent::set_is_anchor);
	ClassDB::bind_method(D_METHOD("get_launch_strength"), &GrappleComponent::get_launch_strength);
	ClassDB::bind_method(D_METHOD("set_launch_strength", "val"), &GrappleComponent::set_launch_strength);
	ClassDB::bind_method(D_METHOD("get_mass"), &GrappleComponent::get_mass);
	ClassDB::bind_method(D_METHOD("set_mass", "val"), &GrappleComponent::set_mass);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "anchored"), "set_is_anchor", "get_is_anchor");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "launch strength"), "set_launch_strength", "get_launch_strength");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mass"), "set_mass", "get_mass");
}

void GrappleComponent::_enter_tree() {
	if (!get_parent_node<RigidBody3D>() && !get_parent_node<CharacterBody3D>() && !anchored) {
		anchored = true;
		LOG(WARN, "A Node that is not inheriting from either Rigidbody or CharacterBody3D must be an anchor");
	}

	if (!m_area) { m_area = get_child_node_of_type<Area3D>(this); }
	if (m_area && !m_shape) { m_shape = get_child_node_of_type<CollisionShape3D>(m_area); }
	if (!m_area) { LOG(DEBUG, "Grapplecomponent missing area,", get_name()) }
	if (!m_shape) { LOG(DEBUG, "Grapplecomponent missing shape,", get_name()) }
}

void GrappleComponent::_exit_tree() {
	m_area = nullptr;
	m_shape = nullptr;
}

RID GrappleComponent::get_rid() const {
	if (!m_area) {
		return RID();
	}
	return m_area->get_rid();
}

void GrappleComponent::set_is_anchor(bool val) { anchored = val; }

bool GrappleComponent::get_is_anchor() const { return anchored; }

void GrappleComponent::set_launch_strength(float val) { m_launch_strength = val; }

float GrappleComponent::get_launch_strength() const { return m_launch_strength; }

void GrappleComponent::set_mass(float val) { m_mass = val; }

float GrappleComponent::get_mass() const { return m_mass; }
