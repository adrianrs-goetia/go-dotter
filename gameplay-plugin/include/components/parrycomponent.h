#pragma once

#include <core/core.h>
#include <core/nodecomponent.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>

using namespace godot;

class ParryComponent : public NodeComponent {
	GDCLASS(ParryComponent, NodeComponent)

public:
	Area3D* m_area = nullptr;
	CollisionShape3D* m_collisionshape = nullptr; // TODO: superfluous ptr?
	TypedArray<RID> m_rid_ignores;

	TypedArray<Vector3> m_colliding_positions;

public:
	GETNAME(ParryComponent)
	static void _bind_methods();

	void _enter_tree() override;
	void _physics_process(double delta) override;

	TypedArray<Vector3> get_colliding_positions() const;
	Vector3 get_closest_colliding_position() const;

private:
	TypedArray<Vector3> _get_collision_query() const;
	Ref<PhysicsShapeQueryParameters3D> _get_physicsshapequeryparams() const;


	template <typename T>
	T *get_child_node_of_type(const godot::Node *node) {
		TypedArray<godot::Node> children = node->get_children();
		for (int i = 0; i < children.size(); ++i) {
			if (T *child = cast_to<T>(children[i])) {
				return child;
			}
		}
		return nullptr;
	}
};