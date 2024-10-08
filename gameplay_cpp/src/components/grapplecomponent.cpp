#include <components/grapplecomponent.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <debugdraw3d/api.h>

GrappleComponent::LaunchContext GrappleComponent::launch(GrappleComponent* subject) {
	LaunchContext context;
	context.type = _determine_launch_type(subject);
	switch (context.type) {
		case LaunchType::BOTH_ANCHOR: {
			LOG(WARN, "GrappleComponent launch between two anchors. This shouldn't happen")
			break;
		}
		case LaunchType::BOTH_NON_ANCHOR: {
			const float max_mass = get_mass() + subject->get_mass();
			const float subject_weight = get_mass() / max_mass;
			const float instigator_weight = subject->get_mass() / max_mass;
			LOG(DEBUG, "Grapple launch -- BOTH_NON_ANCHOR")
			std::ignore = subject->_impulse_owner(
					_determine_launch_direction_atob(subject, this), get_pull_strength() * subject_weight);
			context.impulse = _impulse_owner(
					_determine_launch_direction_atob(this, subject), get_pull_strength() * instigator_weight);
			break;
		}
		case LaunchType::INSTIGATOR_ANCHOR: {
			LOG(DEBUG, "Grapple launch -- INSTIGATOR_ANCHOR")
			std::ignore = subject->_impulse_owner(_determine_launch_direction_atob(subject, this), get_pull_strength());
			break;
		}
		case LaunchType::SUBJECT_ANCHOR: {
			LOG(DEBUG, "Grapple launch -- SUBJECT_ANCHOR")
			context.impulse = _impulse_owner(_determine_launch_direction_atob(this, subject), get_pull_strength());
			break;
		}
	}
	return context;
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

Vector3 GrappleComponent::_impulse_owner(const Vector3& direction, float impulse_strength) {
	const Vector3 impulse = direction * impulse_strength;
	DebugDraw::Line(get_global_position(), get_global_position() + (impulse), Color(1, 0, 1), 1.f);
	if (auto* p = get_parent_node<RigidBody3D>()) { p->set_linear_velocity(impulse); }
	else if (auto* p = get_parent_node<CharacterBody3D>()) { p->set_velocity(impulse); }
	else {
		LOG(ERROR, "GrappleComponent impulse on invalid parent",
				get_parent()->get_name().get_basename().utf8().get_data())
	}
	return impulse;
}

void GrappleComponent::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("get_is_anchor"), &GrappleComponent::get_is_anchor);
	godot::ClassDB::bind_method(D_METHOD("set_is_anchor", "val"), &GrappleComponent::set_is_anchor);
	godot::ClassDB::bind_method(D_METHOD("get_pull_strength"), &GrappleComponent::get_pull_strength);
	godot::ClassDB::bind_method(D_METHOD("set_pull_strength", "val"), &GrappleComponent::set_pull_strength);
	godot::ClassDB::bind_method(D_METHOD("get_mass"), &GrappleComponent::get_mass);
	godot::ClassDB::bind_method(D_METHOD("set_mass", "val"), &GrappleComponent::set_mass);
	godot::ClassDB::bind_method(D_METHOD("getAreaPath"), &GrappleComponent::getAreaPath);
	godot::ClassDB::bind_method(D_METHOD("setAreaPath", "path"), &GrappleComponent::setAreaPath);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "anchored"), "set_is_anchor", "get_is_anchor");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pull strength"), "set_pull_strength", "get_pull_strength");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mass"), "set_mass", "get_mass");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "Collision AreaPath"), "setAreaPath", "getAreaPath");
}

void GrappleComponent::_enter_tree() {
	if (!get_parent_node<RigidBody3D>() && !get_parent_node<CharacterBody3D>() && !anchored) {
		anchored = true;
		LOG(WARN, "A Node that is not inheriting from either Rigidbody or CharacterBody3D must be an anchor");
	}

	m_area = get_node<Area3D>(m_pathToArea3D);
	if (!m_area) {
		String msg;
		msg += get_parent()->get_name();
		msg += ", failed to fetch m_areaPtr for collision. Please assign a path to a CollisionShape3D to 'Area Path'";
		ERR_PRINT_ONCE(msg);
	}
}

void GrappleComponent::_exit_tree() { m_area = nullptr; }

RID GrappleComponent::get_rid() const {
	if (!m_area) { return RID(); }
	return m_area->get_rid();
}

void GrappleComponent::set_is_anchor(bool val) {
	if (get_parent() && !get_parent_node<RigidBody3D>() && !get_parent_node<CharacterBody3D>() && !val) {
		anchored = true;
		LOG(WARN, "A Node that is not inheriting from either Rigidbody or CharacterBody3D must be an anchor");
		return;
	}
	anchored = val;
}

bool GrappleComponent::get_is_anchor() const { return anchored; }

void GrappleComponent::set_pull_strength(float val) { m_pull_strength = val; }

float GrappleComponent::get_pull_strength() const { return m_pull_strength; }

void GrappleComponent::set_mass(float val) { m_mass = val; }

float GrappleComponent::get_mass() const { return m_mass; }

void GrappleComponent::setAreaPath(NodePath path) { m_pathToArea3D = path; }

NodePath GrappleComponent::getAreaPath() { return m_pathToArea3D; }