#ifndef GD_CHARACTER_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_CHARACTER_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <character/playerstates.h>
#include <core/core.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class PlayerNode : public CharacterBody3D {
	GDCLASS(PlayerNode, CharacterBody3D)

public: // data
	PlayerFSM m_fsm;
	StateContext* m_state_context = nullptr;

public: // functions
	GETNAME(PlayerNode)
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;
	void _process(float delta) override;
	void _physics_process(float delta) override;
	void _input(const Ref<InputEvent>& p_event);
};

#endif // GD_CHARACTER_PLAYERNODE_PLUGIN_STEIKEMANNGAMEPLAY_H