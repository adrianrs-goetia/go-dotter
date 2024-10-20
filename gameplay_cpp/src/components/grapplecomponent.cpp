#include <components/grapplecomponent.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <debugdraw3d/api.h>

GrappleComponent::LaunchContext GrappleComponent::launch(GrappleComponent* subject) {
	LaunchContext context;
	context.type = _determineLaunchType(subject);
	switch (context.type) {
		case LaunchType::BOTH_ANCHOR: {
			LOG(WARN, "GrappleComponent launch between two anchors. This shouldn't happen")
			break;
		}
		case LaunchType::BOTH_NON_ANCHOR: {
			const float max_mass = getMass() + subject->getMass();
			const float subject_weight = getMass() / max_mass;
			const float instigator_weight = subject->getMass() / max_mass;
			LOG(DEBUG, "Grapple launch -- BOTH_NON_ANCHOR")
			std::ignore = subject->_impulseOwner(
					_determineLaunchDirectionAtob(subject, this), getPullStrength() * subject_weight);
			context.impulse = _impulseOwner(
					_determineLaunchDirectionAtob(this, subject), getPullStrength() * instigator_weight);
			break;
		}
		case LaunchType::INSTIGATOR_ANCHOR: {
			LOG(DEBUG, "Grapple launch -- INSTIGATOR_ANCHOR")
			std::ignore = subject->_impulseOwner(_determineLaunchDirectionAtob(subject, this), getPullStrength());
			break;
		}
		case LaunchType::SUBJECT_ANCHOR: {
			LOG(DEBUG, "Grapple launch -- SUBJECT_ANCHOR")
			context.impulse = _impulseOwner(_determineLaunchDirectionAtob(this, subject), getPullStrength());
			break;
		}
	}
	return context;
}

GrappleComponent::LaunchType GrappleComponent::_determineLaunchType(const GrappleComponent* subject) {
	if (getIsAnchor() && subject->getIsAnchor()) { return LaunchType::BOTH_ANCHOR; }
	else if (!getIsAnchor() && subject->getIsAnchor()) { return LaunchType::SUBJECT_ANCHOR; }
	else if (getIsAnchor() && !subject->getIsAnchor()) { return LaunchType::INSTIGATOR_ANCHOR; }
	return LaunchType::BOTH_NON_ANCHOR;
}

Vector3 GrappleComponent::_determineLaunchDirectionAtob(const GrappleComponent* a, const GrappleComponent* b) {
	return Vector3(b->get_global_position() - a->get_global_position()).normalized();
}

Vector3 GrappleComponent::_impulseOwner(const Vector3& direction, float impulse_strength) {
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
	godot::ClassDB::bind_method(D_METHOD("getIsAnchor"), &GrappleComponent::getIsAnchor);
	godot::ClassDB::bind_method(D_METHOD("setIsAnchor", "val"), &GrappleComponent::setIsAnchor);
	godot::ClassDB::bind_method(D_METHOD("getPullStrength"), &GrappleComponent::getPullStrength);
	godot::ClassDB::bind_method(D_METHOD("setPullStrength", "val"), &GrappleComponent::setPullStrength);
	godot::ClassDB::bind_method(D_METHOD("getMass"), &GrappleComponent::getMass);
	godot::ClassDB::bind_method(D_METHOD("setMass", "val"), &GrappleComponent::setMass);
	godot::ClassDB::bind_method(D_METHOD("getAreaPath"), &GrappleComponent::getAreaPath);
	godot::ClassDB::bind_method(D_METHOD("setAreaPath", "path"), &GrappleComponent::setAreaPath);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "anchored"), "setIsAnchor", "getIsAnchor");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pull strength"), "setPullStrength", "getPullStrength");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mass"), "setMass", "getMass");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "Collision AreaPath"), "setAreaPath", "getAreaPath");
}

void GrappleComponent::_enter_tree() {
	if (!get_parent_node<RigidBody3D>() && !get_parent_node<CharacterBody3D>() && !m_anchored) {
		m_anchored = true;
		LOG(WARN, "A Node that is not inheriting from either Rigidbody or CharacterBody3D must be an anchor");
	}

	RETURN_IF_EDITOR
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

void GrappleComponent::_exit_tree() { m_area = nullptr; }

RID GrappleComponent::getRid() const {
	if (!m_area) { return RID(); }
	return m_area->get_rid();
}

void GrappleComponent::setIsAnchor(bool val) {
	if (get_parent() && !get_parent_node<RigidBody3D>() && !get_parent_node<CharacterBody3D>() && !val) {
		m_anchored = true;
		LOG(WARN, "A Node that is not inheriting from either Rigidbody or CharacterBody3D must be an anchor");
		return;
	}
	m_anchored = val;
}

bool GrappleComponent::getIsAnchor() const { return m_anchored; }

void GrappleComponent::setPullStrength(float val) { m_pullStrength = val; }

float GrappleComponent::getPullStrength() const { return m_pullStrength; }

void GrappleComponent::setMass(float val) { m_mass = val; }

float GrappleComponent::getMass() const { return m_mass; }

void GrappleComponent::setAreaPath(NodePath path) { m_pathToArea3D = path; }

NodePath GrappleComponent::getAreaPath() { return m_pathToArea3D; }