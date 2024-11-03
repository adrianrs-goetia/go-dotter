#ifndef GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H

#include <core/core.h>
#include <character/playerFsm.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/spring_arm3d.hpp>

class PlayerNode;

class CameraPivot : public godot::SpringArm3D {
	GDCLASS(CameraPivot, godot::SpringArm3D)

public:
public:
	static void _bind_methods();

	void _ready() override;
	void _enter_tree() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

	// void set_length_impl(float p_length);
	// float get_length_impl() const;

	void process(StateContext& context, float delta);
	void processInput(StateContext& context, float delta);
};

#endif // GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H