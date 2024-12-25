#pragma once

#include "baseClasses/grappleBase.hpp"
#include "grappleTarget.hpp"

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <managers/inputManager.h>

#include <debugdraw3d/api.h>
#include <configparams.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include <algorithm>

class ComponentGrappleInstigator : public ComponentGrappleBase {
	GDCLASS(ComponentGrappleInstigator, ComponentGrappleBase)

	using InstigatorDirection = std::function<godot::Vector3(const Node& node)>;
	using InRangeTargetMap = std::map<godot::RID, ComponentGrappleTarget*>;

private:
	GS_PATH_IMPL(m_colliderPath, ColliderPath)
	godot::Area3D* m_detectionArea = nullptr;

	/**
	 * Shared ptr to allow for weakptrs in NodeComponent onDestruction callback.
	 * No other object is meant to have a shared ptr to this field
	 *
	 * godot::Node are not refcounted so each intance that is transient cannot be blindly derefenced
	 * and so it has to 'track' its own entry within this map and remove it onDestruction.
	 */
	std::shared_ptr<InRangeTargetMap> m_inRangeTargets; // Should never be nullptrs
	std::set<godot::RID> m_ignoredRids;
	InstigatorDirection m_getInstigatorDirection;

	ComponentGrappleTarget* m_currentTarget = nullptr;

public:
	static void _bind_methods() {
		METHOD_INOUT_BIND(ComponentGrappleInstigator, areaEnteredDetection, areaExitedDetection, area)
		METHOD_PROPERTY_IMPL(ComponentGrappleInstigator, ColliderPath, NODE_PATH)
	}

	void setComponentEnabled(bool enabled) override {
		if (!_setComponentEnabledImpl(enabled)) {
			return;
		}

		if (!enabled) {
			m_currentTarget = nullptr;
			m_inRangeTargets->clear();
		}
	}

	void _enter_tree() override {
		set_name(get_class());

		RETURN_IF_EDITOR(void())

		m_inRangeTargets = std::make_shared<InRangeTargetMap>();
		m_detectionArea = get_node<Area3D>(m_colliderPath);

		ASSERTNN(m_detectionArea)

		m_detectionArea->connect("area_entered", callable_mp(this, &ComponentGrappleInstigator::areaEnteredDetection));
		m_detectionArea->connect("area_exited", callable_mp(this, &ComponentGrappleInstigator::areaExitedDetection));
	}

	void _physics_process(double delta) override {
		RETURN_IF_EDITOR(void())

		// maybe a callback instead of checking each frame?
		setComponentEnabled(
			ConfigParam::Player::Grapple::enabled()); // @TODO, make this function into cb that owner has to
													  // set. Do NOT use specified path like this.. yuk
		if (!isComponentEnabled()) {
			return;
		}

		determineTarget();

		// @TODO remove
		// tmp targeting system for grappling
		if (getTarget()) {
			DebugDraw::Position(
				Transform3D(Basis(Vector3(0, 1, 0), 0, Vector3(3, 3, 3)), getTarget()->get_global_position()),
				Color(0, 0, 1),
				delta);
			DebugDraw::Line(get_global_position(), getTarget()->get_global_position(), Color(0, 1, 0));
		}
	}

	void areaEnteredDetection(godot::Area3D* area) {
		RETURN_IF_EDITOR(void())
		if (auto* gn = getAdjacentNode<ComponentGrappleTarget>(area)) {
			auto rid = gn->getRid();
			m_inRangeTargets->insert({ rid, gn });
			auto wp = std::weak_ptr<InRangeTargetMap>(m_inRangeTargets);
			gn->addOnDestructionCb(NodeComponent::DestructionId::GRAPPLECOMPONENT_INRANGE,
				[wp, rid]()
				{
					auto lock = wp.lock();
					if (lock) {
						lock->erase(rid);
					}
				});
		}
	}

	void areaExitedDetection(godot::Area3D* area) {
		RETURN_IF_EDITOR(void())
		if (auto* gn = getAdjacentNode<ComponentGrappleTarget>(area)) {
			m_inRangeTargets->erase(gn->getRid());
			if (gn == m_currentTarget) {
				m_currentTarget = nullptr;
			}
		}
	}

	void determineTarget() {
		RETURN_IF_EDITOR(void())
		ASSERTNN(m_getInstigatorDirection,
			"ComponentGrappleInstigator requires setInstigatorDirection to be set to determineTarget")

		const Vector3 cam3d = m_getInstigatorDirection(*this);
		float lowest_dot = -1.0f;
		ComponentGrappleTarget* target = nullptr;
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

	LaunchContext launch(double launchStrength) {
		LaunchContext context;
		auto* subject = getTarget();
		context.type = _determineLaunchType(subject);
		switch (context.type) {
			case LaunchType::BOTH_ANCHOR: {
				LOG(WARN, "ComponentGrappleInstigator launch between two anchors. This shouldn't happen")
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

private:
	LaunchType _determineLaunchType(const ComponentGrappleBase* subject) {
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

	godot::Vector3 _determineLaunchDirectionAtob(const ComponentGrappleBase* a, const ComponentGrappleBase* b) {
		return Vector3(b->get_global_position() - a->get_global_position()).normalized();
	}

public: // getters-setters
	ComponentGrappleTarget* getTarget() const {
		return m_currentTarget;
	}
	void setInstigatorDirection(const InstigatorDirection&& getInstigatorDirection) {
		m_getInstigatorDirection = getInstigatorDirection;
	}
};
