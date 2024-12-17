#pragma once

#include "typedefs.hpp"
#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <functional>
#include <map>
#include <optional>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>

class ComponentParryTarget;
class ParryInstance;

class ComponentParryInstigator : public NodeComponent {
	GDCLASS(ComponentParryInstigator, NodeComponent)

public:
	using GodotRID = uint64_t;

	struct ActivateParams {
		godot::Vector3 direction;
		float length;
		float lift;
	};

public:
	godot::NodePath m_colliderPath;
	godot::Area3D* m_area = nullptr;
	std::map<GodotRID, ComponentParryTarget&> m_inRangeParryTargets;

	std::weak_ptr<ParryContact> m_lastParryContact;

public:
	static void _bind_methods();

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() override;
	void _exit_tree() override;
	void _physics_process(double delta) override;

	void areaEnteredParryDetection(godot::Area3D* area);
	void areaExitedParryDetection(godot::Area3D* area);

	ComponentParryTarget* getLastParryContactAssert() const;

	std::optional<ParryInstance> activateParry(ActivateParams params);

	godot::Vector3 getPosition() const;
	godot::Vector3 getVelocity() const;

	GS_PATH_IMPL(m_colliderPath, ColliderPath)
};