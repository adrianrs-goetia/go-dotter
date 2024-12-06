#include "grappleInstigatorComponent.h"

#include "grappleTargetComponent.h"

#include <managers/inputManager.h>

#include <configHandler.h>
#include <debugdraw3d/api.h>
#include <godot_cpp/classes/area3d.hpp>

#include <algorithm>

// TODO, remove this. CONFIG_PREFIX should never be on any component
#define CONFIG_PREFIX "player"

void GrappleInstigatorComponent::_bind_methods() {
	ClassDB::bind_method(
			D_METHOD("areaEnteredGrappledetection", "area"), &GrappleInstigatorComponent::areaEnteredDetection);
	ClassDB::bind_method(
			D_METHOD("areaExitedGrappledetection", "area"), &GrappleInstigatorComponent::areaExitedDetection);

	METHOD_PROPERTY_IMPL(GrappleInstigatorComponent, PathToArea3D, NODE_PATH, "Detection area")
}

void GrappleInstigatorComponent::setComponentEnabled(bool enabled) {
	if (!_setComponentEnabledImpl(enabled)) {
		return;
	}

	if (!enabled) {
		m_currentTarget = nullptr;
		m_inRangeTargets->clear();
	}
}

void GrappleInstigatorComponent::_enter_tree() {
	set_name(get_class());

	RETURN_IF_EDITOR(void())

	m_inRangeTargets = std::make_shared<InRangeTargetMap>();
	m_detectionArea = get_node<Area3D>(m_pathToGrappleDetectionArea);

	ASSERT_NOTNULL(m_detectionArea)

	m_detectionArea->connect("area_entered", callable_mp(this, &GrappleInstigatorComponent::areaEnteredDetection));
	m_detectionArea->connect("area_exited", callable_mp(this, &GrappleInstigatorComponent::areaExitedDetection));
}

void GrappleInstigatorComponent::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	// maybe a callback instead of checking each frame?
	setComponentEnabled(GETPARAM_B("grapple", "enabled")); // @TODO, make this function into cb that owner has to set.
														   // Do NOT use specified path like this.. yuk
	if (!isComponentEnabled()) {
		return;
	}

	determineTarget();

	// @TODO remove
	// tmp targeting system for grappling
	if (getTarget()) {
		DebugDraw::Position(
				Transform3D(Basis(Vector3(0, 1, 0), 0, Vector3(3, 3, 3)), getTarget()->get_global_position()),
				Color(0, 0, 1), delta);
		DebugDraw::Line(get_global_position(), getTarget()->get_global_position(), Color(0, 1, 0));
	}
}

void GrappleInstigatorComponent::areaEnteredDetection(Area3D* area) {
	RETURN_IF_EDITOR(void())
	if (auto* gn = getAdjacentNode<GrappleTargetComponent>(area)) {
		LOG(DEBUG, "Component entered grapple area: ", gn->get_parent()->get_name())
		auto rid = gn->getRid();
		m_inRangeTargets->insert({ rid, gn });
		auto wp = std::weak_ptr<InRangeTargetMap>(m_inRangeTargets);
		gn->addOnDestructionCb(NodeComponent::DestructionId::GRAPPLECOMPONENT_INRANGE, [wp, rid]() {
			auto lock = wp.lock();
			if (lock) {
				lock->erase(rid);
			}
		});
	}
}

void GrappleInstigatorComponent::areaExitedDetection(Area3D* area) {
	RETURN_IF_EDITOR(void())
	if (auto* gn = getAdjacentNode<GrappleTargetComponent>(area)) {
		LOG(DEBUG, "Node left grapple area: ", area->get_parent()->get_name())
		m_inRangeTargets->erase(gn->getRid());
		if (gn == m_currentTarget) {
			m_currentTarget = nullptr;
		}
	}
}

void GrappleInstigatorComponent::determineTarget() {
	RETURN_IF_EDITOR(void())
	ASSERT_NOTNULL_MSG(m_getInstigatorDirection,
			"GrappleInstigatorComponent requires setInstigatorDirection to be set to determineTarget")

	const Vector3 cam3d = m_getInstigatorDirection(*this);
	float lowest_dot = -1.0f;
	GrappleTargetComponent* target = nullptr;
	for (auto [_, gn] : *m_inRangeTargets) {
		Vector3 dir_2d = gn->get_global_position() - get_global_position();
		dir_2d.y = 0;
		dir_2d.normalize();
		const float dot = cam3d.dot(dir_2d);
		if (dot > lowest_dot && dot > 0.f) {
			lowest_dot = dot;
			target = gn;
		}
	}
	if (target)
		m_currentTarget = target;
	else
		m_currentTarget = nullptr;
}

GrappleInstigatorComponent::LaunchContext GrappleInstigatorComponent::launch(double launchStrength) {
	LaunchContext context;
	auto* subject = getTarget();
	context.type = _determineLaunchType(subject);
	switch (context.type) {
		case LaunchType::BOTH_ANCHOR: {
			LOG(WARN, "GrappleInstigatorComponent launch between two anchors. This shouldn't happen")
			break;
		}
		case LaunchType::BOTH_NON_ANCHOR: {
			const float max_mass = getMass() + subject->getMass();
			const float subject_weight = getMass() / max_mass;
			const float instigator_weight = subject->getMass() / max_mass;
			LOG(DEBUG, "Grapple launch -- BOTH_NON_ANCHOR")
			std::ignore = subject->impulseOwner(
					_determineLaunchDirectionAtob(subject, this), getPullStrength() * subject_weight);
			context.impulse =
					impulseOwner(_determineLaunchDirectionAtob(this, subject), getPullStrength() * instigator_weight);
			break;
		}
		case LaunchType::INSTIGATOR_ANCHOR: {
			LOG(DEBUG, "Grapple launch -- INSTIGATOR_ANCHOR")
			std::ignore = subject->impulseOwner(_determineLaunchDirectionAtob(subject, this), getPullStrength());
			break;
		}
		case LaunchType::SUBJECT_ANCHOR: {
			LOG(DEBUG, "Grapple launch -- SUBJECT_ANCHOR")
			context.impulse = impulseOwner(_determineLaunchDirectionAtob(this, subject), launchStrength);
			break;
		}
	}
	return context;
}

GrappleBaseComponent::LaunchType GrappleInstigatorComponent::_determineLaunchType(const GrappleBaseComponent* subject) {
	if (getIsAnchor() && subject->getIsAnchor()) {
		return LaunchType::BOTH_ANCHOR;
	}
	else if (!getIsAnchor() && subject->getIsAnchor()) {
		return LaunchType::SUBJECT_ANCHOR;
	}
	else if (getIsAnchor() && !subject->getIsAnchor()) {
		return LaunchType::INSTIGATOR_ANCHOR;
	}
	return LaunchType::BOTH_NON_ANCHOR;
}

Vector3 GrappleInstigatorComponent::_determineLaunchDirectionAtob(
		const GrappleBaseComponent* a, const GrappleBaseComponent* b) {
	return Vector3(b->get_global_position() - a->get_global_position()).normalized();
}
