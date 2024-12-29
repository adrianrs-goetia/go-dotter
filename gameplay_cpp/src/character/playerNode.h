#ifndef GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H

#include <managers/inputManager.h>
#include <core/core.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <godot_cpp/classes/physics_direct_body_state3d.hpp>

namespace fsm::player {
class Fsm;
}

using namespace godot;

class CameraPivot;
class ComponentWeapon;
class ComponentAnimation;
class ComponentGrappleTarget;
class ComponentParryInstigator;
class EventParry;

class PlayerNode : public RigidBody3D {
	GDCLASS(PlayerNode, RigidBody3D)

public:
	fsm::player::Fsm* m_fsm = nullptr;
	CameraPivot* m_camerapivot = nullptr;
	ComponentWeapon* m_weaponComponent = nullptr;
	ComponentAnimation* m_animComponent = nullptr;
	ComponentParryInstigator* m_parryComponent = nullptr;
	std::vector<ComponentGrappleTarget*> m_inRangeGrapplenodes;

	static godot::Transform3D startTransform;

public:
	static void _bind_methods();

	void _notification(int what);
	void _enter_tree() override;
	void _exit_tree() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;
	void _input(const Ref<InputEvent>& p_event);
	void _integrate_forces(PhysicsDirectBodyState3D* state);

	void setupGui();
};

#endif // GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H