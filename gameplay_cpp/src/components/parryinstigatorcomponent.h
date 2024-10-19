#pragma once

#include <core/core.h>
#include <core/nodecomponent.hpp>

#include <map>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>

using namespace godot;

class ParryTargetComponent;

class ParryInstigatorComponent : public NodeComponent {
	GDCLASS(ParryInstigatorComponent, NodeComponent)

	using GodotRID = uint64_t;

public:
	NodePath m_pathToArea3D; // Path for m_area object
	Area3D* m_area = nullptr;
	// CollisionShape3D* m_collisionShape = nullptr;
	// TypedArray<RID> m_ridIgnores;
	// TypedArray<Vector3> m_collidingPositions;
	std::map<GodotRID, ParryTargetComponent*> m_inRangeParryTargets;

public:
	GETNAME(ParryInstigatorComponent)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;
	void _physics_process(double delta) override;

	void setPathToArea3D(NodePath path);
	NodePath getPathToArea3D();

	void areaEnteredParryDetection(Area3D* area);
	void areaExitedParryDetection(Area3D* area);

	void activateParry();

	// TypedArray<Vector3> getCollidingPositions() const;
	// Vector3 getClosestCollidingPosition() const;

private:
	// TypedArray<Vector3> _getCollisionQuery() const;
	// Ref<PhysicsShapeQueryParameters3D> _get_physicsshapequeryparams() const;
};