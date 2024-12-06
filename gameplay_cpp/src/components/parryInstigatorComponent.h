#pragma once

#include <core/core.hpp>
#include <core/nodeComponent.hpp>

#include <functional>
#include <map>
#include <optional>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>

class ParryTargetComponent;
class ParryInstance;

class ParryInstigatorComponent : public NodeComponent {
	GDCLASS(ParryInstigatorComponent, NodeComponent)

public:
	using GodotRID = uint64_t;

public:
	godot::NodePath m_pathToArea3D; // Path for m_area object
	godot::Area3D* m_area = nullptr;
	std::map<GodotRID, ParryTargetComponent&> m_inRangeParryTargets;
	std::function<godot::Vector3()> m_getDesiredDirectionCb;
	float m_mass = 1.f;

public:
	static void _bind_methods();

	void setComponentEnabled(bool enabled) override {}

	void _enter_tree() override;
	void _exit_tree() override;
	void _physics_process(double delta) override;

	void areaEnteredParryDetection(godot::Area3D* area);
	void areaExitedParryDetection(godot::Area3D* area);

	std::optional<ParryInstance> activateParry();

	godot::Vector3 getPosition() const;
	godot::Vector3 getVelocity() const;
	godot::Vector3 getDesiredDirection() const;

	GS_PATH_IMPL(m_pathToArea3D, PathToArea3D)
	GS_FLOAT_IMPL(m_mass, Mass)
};