#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>


#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <debugdraw3d/api.h>

class GrappleBaseComponent : public NodeComponent {
	GDCLASS(GrappleBaseComponent, NodeComponent)

public:
	enum LaunchType {
		BOTH_ANCHOR,
		BOTH_NON_ANCHOR,
		INSTIGATOR_ANCHOR,
		SUBJECT_ANCHOR,
	};
	struct LaunchContext {
		godot::Vector3 impulse;
		LaunchType type;
	};

protected:
	bool m_anchored = false;
	float m_pullStrength = 0.f;
	float m_mass = 10.f;

public:
	static void _bind_methods() {
		using namespace godot;
		godot::ClassDB::bind_method(D_METHOD("getIsAnchor"), &GrappleBaseComponent::getIsAnchor);
		godot::ClassDB::bind_method(D_METHOD("setIsAnchor", "val"), &GrappleBaseComponent::setIsAnchor);
		godot::ClassDB::bind_method(D_METHOD("getPullStrength"), &GrappleBaseComponent::getPullStrength);
		godot::ClassDB::bind_method(D_METHOD("setPullStrength", "val"), &GrappleBaseComponent::setPullStrength);
		godot::ClassDB::bind_method(D_METHOD("getMass"), &GrappleBaseComponent::getMass);
		godot::ClassDB::bind_method(D_METHOD("setMass", "val"), &GrappleBaseComponent::setMass);

		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "anchored"), "setIsAnchor", "getIsAnchor");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pull strength"), "setPullStrength", "getPullStrength");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mass"), "setMass", "getMass");
	}

	virtual godot::Vector3 impulseOwner(const godot::Vector3& direction, float impulse_strength) {
		const godot::Vector3 impulse = direction * impulse_strength;
		DebugDraw::Line(get_global_position(), get_global_position() + (impulse), godot::Color(1, 0, 1), 1.f);
		if (auto* p = getParentNode<godot::RigidBody3D>(this)) { p->set_linear_velocity(impulse); }
		else if (auto* p = getParentNode<godot::CharacterBody3D>(this)) { p->set_velocity(impulse); }
		else {
			LOG(ERROR, "GrappleTargetComponent impulse on invalid parent",
					get_parent()->get_name().get_basename().utf8().get_data())
		}
		return impulse;
	}

public: // getters-setters
	void setIsAnchor(bool anchored) {
		if (get_parent() && !getParentNode<godot::RigidBody3D>(this) && !getParentNode<godot::CharacterBody3D>(this) &&
				!anchored) {
			m_anchored = true;
			LOG(WARN, "A Node that is not inheriting from either Rigidbody or CharacterBody3D must be an anchor");
			return;
		}
		m_anchored = anchored;
	}

	bool getIsAnchor() const { return m_anchored; }

	void setPullStrength(float pullStrength) { m_pullStrength = pullStrength; }

	float getPullStrength() const { return m_pullStrength; }

	void setMass(float mass) { m_mass = mass; }

	float getMass() const { return m_mass; }
};