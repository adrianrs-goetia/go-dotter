#ifndef GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H

#include <character/playerstates.h>
#include <components/inputcomponent.h>
#include <core/core.h>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>

using namespace godot;

class CameraPivot;
class GrappleComponent;
class ParryComponent;

class PlayerNode : public CharacterBody3D {
	GDCLASS(PlayerNode, CharacterBody3D)

public:
	PlayerFSM m_fsm;
	StateContext* m_state_context = nullptr;
	CameraPivot* m_camerapivot = nullptr;
	Node3D* meshAnchor = nullptr;
	// CollisionShape3D* m_capsule = nullptr;
	Area3D* m_grappledetectionarea = nullptr;

	// Area3D* m_parrydetectionarea = nullptr;
	// CollisionShape3D* m_parrydetectionshape = nullptr;

	GrappleComponent* m_grapplecomponent = nullptr;
	ParryComponent* m_parrycomponent = nullptr;

	std::vector<GrappleComponent*> m_in_range_grapplenodes;

public:
	GETNAME(PlayerNode)
	static void _bind_methods();

	void _notification(int what);
	void _enter_tree() override;
	void _exit_tree() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;
	void _input(const Ref<InputEvent>& p_event);

	void rotate_towards_velocity(float delta);

	void area_entered_grappledetection(Area3D* area);
	void area_exited_grappledetection(Area3D* area);
	void determine_grapple_target();


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

#endif // GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H