#include "grappleInstigatorComponent.h"

#include <components/grappleTargetComponent.h>
#include <managers/inputManager.h>

#include <debugdraw3d/api.h>
#include <godot_cpp/classes/area3d.hpp>

#include <algorithm>

void GrappleInstigatorComponent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("getPathToArea3D"), &GrappleInstigatorComponent::getPathToArea3D);
	ClassDB::bind_method(D_METHOD("setPathToArea3D", "path"), &GrappleInstigatorComponent::setPathToArea3D);
	ClassDB::bind_method(
			D_METHOD("areaEnteredGrappledetection", "area"), &GrappleInstigatorComponent::areaEnteredDetection);
	ClassDB::bind_method(
			D_METHOD("areaExitedGrappledetection", "area"), &GrappleInstigatorComponent::areaExitedDetection);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "Detection area"), "setPathToArea3D", "getPathToArea3D");
}

void GrappleInstigatorComponent::_enter_tree() {
	set_name(get_class());

	RETURN_IF_EDITOR(void())

	m_instigatorsGrappleComponent = getAdjacentNode<GrappleTargetComponent>(this);
	m_detectionArea = get_node<Area3D>(m_pathToGrappleDetectionArea);

	ASSERT_NOTNULL(m_instigatorsGrappleComponent)
	ASSERT_NOTNULL(m_detectionArea)

	m_detectionArea->connect("area_entered", callable_mp(this, &GrappleInstigatorComponent::areaEnteredDetection));
	m_detectionArea->connect("area_exited", callable_mp(this, &GrappleInstigatorComponent::areaExitedDetection));
}

void GrappleInstigatorComponent::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	determineTarget();
	if (getTarget()) {
		DebugDraw::Position(
				Transform3D(Basis(Vector3(0, 1, 0), 0, Vector3(3, 3, 3)), getTarget()->get_global_position()),
				Color(0, 0, 1), delta);
		DebugDraw::Line(get_global_position(), getTarget()->get_global_position(), Color(0, 1, 0));
	}
}

void GrappleInstigatorComponent::areaEnteredDetection(Area3D* area) {
	RETURN_IF_EDITOR(void())
	// if (area->get_rid() == getRid()) { return; }
	if (auto* gn = getAdjacentNode<GrappleTargetComponent>(area)) {
		LOG(DEBUG, "Component entered grapple area: ", gn->get_name())
		m_inRangeTargets.push_back(gn);
	}
}

void GrappleInstigatorComponent::areaExitedDetection(Area3D* area) {
	RETURN_IF_EDITOR(void())
	if (auto* gn = getAdjacentNode<GrappleTargetComponent>(area)) {
		LOG(DEBUG, "Node left grapple area: ", area->get_parent()->get_name())
		auto it = std::find_if(m_inRangeTargets.begin(), m_inRangeTargets.end(),
				[gn](GrappleTargetComponent* a) -> bool { return a->getRid() == gn->getRid(); });
		m_inRangeTargets.erase(it);
		if (gn == m_currentTarget) { m_currentTarget = nullptr; }
	}
}

void GrappleInstigatorComponent::determineTarget() {
	RETURN_IF_EDITOR(void())
	ASSERT_NOTNULL_MSG(m_getInstigatorDirection,
			"GrappleInstigatorComponent requires setInstigatorDirection to be set to determineTarget")

	const Vector3 cam3d = m_getInstigatorDirection(*this);
	float lowest_dot = -1.0f;
	GrappleTargetComponent* target = nullptr;
	for (GrappleTargetComponent* gn : m_inRangeTargets) {
		Vector3 dir_2d = gn->get_global_position() - get_global_position();
		dir_2d.y = 0;
		dir_2d.normalize();
		const float dot = cam3d.dot(dir_2d);
		if (dot > lowest_dot && dot > 0.f) {
			lowest_dot = dot;
			target = gn;
		}
	}
	if (target) m_currentTarget = target;
	else m_currentTarget = nullptr;
}
