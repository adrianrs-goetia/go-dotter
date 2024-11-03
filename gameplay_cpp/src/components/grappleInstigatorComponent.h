#pragma once

#include <core/core.h>

#include <core/nodeComponent.hpp>

#include <functional>
#include <vector>

class GrappleComponent;
namespace godot {
	class Area3D;
}

class GrappleInstigatorComponent : public NodeComponent {
	GDCLASS(GrappleInstigatorComponent, NodeComponent)

    using InstigatorDirection = std::function<godot::Vector3(const Node& node)>;
    
private:
	NodePath m_pathToGrappleDetectionArea;
	Area3D* m_detectionArea = nullptr;

	std::vector<GrappleComponent*> m_inRangeTargets; // Should never be nullptrs
	std::set<RID> m_ignoredRids;
	InstigatorDirection m_getInstigatorDirection;

	GrappleComponent* m_instigatorsGrappleComponent =
			nullptr; // tmp workaround to keep current shared instigator/target implementation of grappleLaunch
	GrappleComponent* m_currentTarget = nullptr;

public:
	GETNAME(GrappleInstigatorComponent)
	static void _bind_methods();

	void _enter_tree() override;
	void _physics_process(double delta) override;

	void areaEnteredDetection(Area3D* area);
	void areaExitedDetection(Area3D* area);
	void determineTarget();

public: // getters-setters
	GrappleComponent* getTarget() const { return m_currentTarget; }
	GrappleComponent* getInstigatorComponent() const { return m_instigatorsGrappleComponent; }
	void setInstigatorDirection(const InstigatorDirection&& getInstigatorDirection) {
		m_getInstigatorDirection = getInstigatorDirection;
	}
	void setPathToArea3D(NodePath path) { m_pathToGrappleDetectionArea = path; }
	NodePath getPathToArea3D() const { return m_pathToGrappleDetectionArea; }
};