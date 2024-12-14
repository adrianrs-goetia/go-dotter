#ifndef GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H

#include <character/playerFsm.hpp>
#include <core/core.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/spring_arm3d.hpp>

class PlayerNode;

class CameraPivot : public godot::SpringArm3D {
	GDCLASS(CameraPivot, godot::SpringArm3D)

public:
	static void _bind_methods();

	void _physics_process(double delta) override;

	void process(StateContext& context, float delta);
	void processInput(StateContext& context, float delta);

private:
	void _rotation(StateContext& context, float xMulti, float yMulti, float delta);
};

#endif // GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H