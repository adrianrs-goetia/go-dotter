#pragma once

#include <core/core.hpp>

#include <core/nodeComponent.hpp>

#include <functional>
#include <vector>

class GrappleTargetComponent;
namespace godot {
class Area3D;
}

class GrappleInstigatorComponent : public NodeComponent {
	GDCLASS(GrappleInstigatorComponent, NodeComponent)

	using InstigatorDirection = std::function<godot::Vector3(const Node& node)>;

private:
	godot::NodePath m_pathToGrappleDetectionArea;
	godot::Area3D* m_detectionArea = nullptr;

	std::vector<GrappleTargetComponent*> m_inRangeTargets; // Should never be nullptrs
	std::set<godot::RID> m_ignoredRids;
	InstigatorDirection m_getInstigatorDirection;

	GrappleTargetComponent* m_instigatorsGrappleComponent =
			nullptr; // tmp workaround to keep current shared instigator/target implementation of grappleLaunch
	GrappleTargetComponent* m_currentTarget = nullptr;

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _physics_process(double delta) override;

	void areaEnteredDetection(godot::Area3D* area);
	void areaExitedDetection(godot::Area3D* area);
	void determineTarget();

public: // getters-setters
	GrappleTargetComponent* getTarget() const { return m_currentTarget; }
	GrappleTargetComponent* getInstigatorComponent() const { return m_instigatorsGrappleComponent; }
	void setInstigatorDirection(const InstigatorDirection&& getInstigatorDirection) {
		m_getInstigatorDirection = getInstigatorDirection;
	}
	void setPathToArea3D(godot::NodePath path) { m_pathToGrappleDetectionArea = path; }
	godot::NodePath getPathToArea3D() const { return m_pathToGrappleDetectionArea; }
};