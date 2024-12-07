#pragma once

#include "baseClasses/grappleBaseComponent.hpp"

#include "grappleTargetComponent.h"

#include <functional>
#include <map>
#include <memory>
#include <vector>

class GrappleTargetComponent;
namespace godot {
class Area3D;
}

class GrappleInstigatorComponent : public GrappleBaseComponent {
	GDCLASS(GrappleInstigatorComponent, GrappleBaseComponent)

	using InstigatorDirection = std::function<godot::Vector3(const Node& node)>;
	using InRangeTargetMap = std::map<godot::RID, GrappleTargetComponent*>;

private:
	godot::NodePath m_colliderPath;
	godot::Area3D* m_detectionArea = nullptr;

	/**
	 * Shared ptr to allow for weakptrs in NodeComponent onDestruction callback.
	 * No other object is meant to have a shared ptr to this field
	 *
	 * godot::Node are not refcounted so each intance that is transient cannot be blindly derefenced
	 * and so it has to 'track its own' entry within this map and remove it onDestruction.
	 */
	std::shared_ptr<InRangeTargetMap> m_inRangeTargets; // Should never be nullptrs
	std::set<godot::RID> m_ignoredRids;
	InstigatorDirection m_getInstigatorDirection;

	GrappleTargetComponent* m_currentTarget = nullptr;

public:
	static void _bind_methods();

	void setComponentEnabled(bool enabled) override;

	void _enter_tree() override;
	void _physics_process(double delta) override;

	void areaEnteredDetection(godot::Area3D* area);
	void areaExitedDetection(godot::Area3D* area);
	void determineTarget();

	LaunchContext launch(double launchStrength);

private:
	LaunchType _determineLaunchType(const GrappleBaseComponent* subject);
	godot::Vector3 _determineLaunchDirectionAtob(const GrappleBaseComponent* a, const GrappleBaseComponent* b);

public: // getters-setters
	GrappleTargetComponent* getTarget() const {
		return m_currentTarget;
	}
	void setInstigatorDirection(const InstigatorDirection&& getInstigatorDirection) {
		m_getInstigatorDirection = getInstigatorDirection;
	}

	GS_PATH_IMPL(m_colliderPath, ColliderPath)
};
