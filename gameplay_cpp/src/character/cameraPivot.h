#ifndef GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H

#include <core/core.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/spring_arm3d.hpp>

#include <character/fsm/typedefs.hpp>

#include <configparams.hpp>

class PlayerNode;

class CameraPivot : public godot::SpringArm3D {
	GDCLASS(CameraPivot, godot::SpringArm3D)

	ConfigParam::Camera param;

public:
	static void _bind_methods();

	void _physics_process(double delta) override;

	void process(const fsm::player::Context& context, float delta);
	void processInput(const fsm::player::Context& context, float delta);

private:
	void _rotation(const fsm::player::Context& context, float xMulti, float yMulti, float delta);
};

#endif // GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H