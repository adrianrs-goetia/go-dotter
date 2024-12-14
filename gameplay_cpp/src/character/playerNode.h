#ifndef GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H

#include <managers/inputManager.h>
#include <core/core.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>

namespace fsm::player{
	class Fsm;
}

using namespace godot;

class CameraPivot;
class AnimationComponent;
class GrappleTargetComponent;
class ParryInstigatorComponent;
class ParryInstance;

class PlayerNode : public CharacterBody3D {
	GDCLASS(PlayerNode, CharacterBody3D)

public:
	fsm::player::Fsm* m_fsm = nullptr;
	CameraPivot* m_camerapivot = nullptr;
	AnimationComponent* m_animComponent = nullptr;

	ParryInstigatorComponent* m_parryComponent = nullptr;

	std::vector<GrappleTargetComponent*> m_inRangeGrapplenodes;

public:
	static void _bind_methods();

	void _notification(int what);
	void _enter_tree() override;
	void _exit_tree() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;
	void _input(const Ref<InputEvent>& p_event);
};

#endif // GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H