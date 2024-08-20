#ifndef GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H

#include <character/playerstates.h>
#include <core/core.h>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>

using namespace godot;

class CameraPivot;
class GrappleNode;

class PlayerNode : public CharacterBody3D {
	GDCLASS(PlayerNode, CharacterBody3D)

public:
	PlayerFSM m_fsm;
	StateContext* m_state_context = nullptr;
	CameraPivot* m_camerapivot = nullptr;
	Node3D* meshdummy = nullptr;
	CollisionShape3D* m_capsule = nullptr;
	Area3D* m_grappledetectionarea = nullptr;
	Area3D* m_parrydetectionarea = nullptr;
	CollisionShape3D* m_parrydetectionshape = nullptr;

	std::vector<GrappleNode*> m_in_range_grapplenodes;

public:
	GETNAME(PlayerNode)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;
	void _process(float delta) override;
	void _physics_process(float delta) override;
	void _input(const Ref<InputEvent>& p_event);

	void rotate_towards_velocity(float delta);

	void area_entered_grappledetection(Area3D* area);
	void area_exited_grappledetection(Area3D* area);
	void determine_grapple_target();

	void area_entered_parrydetection(Area3D* area);
	void area_exited_parrydetection(Area3D* area);
};

#endif // GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H