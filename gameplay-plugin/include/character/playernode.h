#ifndef GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H

#include <character/playerstates.h>
#include <core/core.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/area3d.hpp>

using namespace godot;

class CameraPivot;

class PlayerNode : public CharacterBody3D {
	GDCLASS(PlayerNode, CharacterBody3D)

public:
	PlayerFSM m_fsm;
	StateContext* m_state_context = nullptr;
	CameraPivot* m_camerapivot = nullptr;
	Node3D* meshdummy = nullptr;
	Area3D* area3d = nullptr;

public:
	GETNAME(PlayerNode)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;
	void _process(float delta) override;
	void _physics_process(float delta) override;
	void _input(const Ref<InputEvent>& p_event);

	void rotate_towards_velocity(float delta);
	
	void body_entered_area3d(Node3D* body);
	void body_left_area3d(Node3D* body);	
	void area_entered_area3d(Area3D* area);
	void area_left_area3d(Area3D* area);	
};

#endif // GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H