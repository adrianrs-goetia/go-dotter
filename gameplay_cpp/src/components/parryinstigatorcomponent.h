#pragma once

#include <core/core.h>
#include <core/nodecomponent.hpp>

#include <map>
#include <functional>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>

using namespace godot;

class ParryTargetComponent;

class ParryInstigatorComponent : public NodeComponent {
	GDCLASS(ParryInstigatorComponent, NodeComponent)

public:
	using GodotRID = uint64_t;

public:
	NodePath m_pathToArea3D; // Path for m_area object
	Area3D* m_area = nullptr;
	std::map<GodotRID, ParryTargetComponent&> m_inRangeParryTargets;
	std::function<Vector3()> m_getDesiredDirectionCb;
	float m_mass = 1.f;

public:
	GETNAME(ParryInstigatorComponent)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;
	void _physics_process(double delta) override;

	void setPathToArea3D(NodePath path);
	NodePath getPathToArea3D() const;
	void setMass(float mass);
	float getMass() const;

	void areaEnteredParryDetection(Area3D* area);
	void areaExitedParryDetection(Area3D* area);

	bool activateParry();

	Vector3 getPosition() const;
	Vector3 getVelocity() const;
	Vector3 getDesiredDirection() const;
};