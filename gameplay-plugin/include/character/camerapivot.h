#ifndef GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H

#include <core/core.h>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/spring_arm3d.hpp>

using namespace godot;

class PlayerNode;

class CameraPivot : public SpringArm3D {
	GDCLASS(CameraPivot, SpringArm3D)

public:
	PlayerNode* m_playernode = nullptr;
	EInputMode m_inputmode;
	Vector2 m_inputmotion;

public:
	GETNAME(CameraPivot)
	static void _bind_methods();

	void _enter_tree() override;
	void _process(float delta) override;
	void _physics_process(float delta) override;
	void _input(const Ref<InputEvent>& p_event);
};

#endif // GD_CHARACTER_CAMERAPIVOT_PLUGIN_GAMEPLAY_H