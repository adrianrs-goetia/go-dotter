#ifndef GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H

#include <core/core.h>
#include <character/playerfsm.h>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/spring_arm3d.hpp>

using namespace godot;

class PlayerNode;

class CameraPivot : public SpringArm3D {
	GDCLASS(CameraPivot, SpringArm3D)

public:

public:
	GETNAME(CameraPivot)
	static void _bind_methods();

	void _enter_tree() override;
	void _physics_process(float delta) override;

	void process(StateContext* context, float delta);
	void process_input(StateContext* context, Input* input, const Ref<InputEvent>& p_event, float delta);
};

#endif // GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H