#ifndef GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H

#include <character/playerstates.h>
#include <components/inputcomponent.h>
#include <core/core.h>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>

using namespace godot;

class CameraPivot;
class GrappleComponent;
class ParryInstigatorComponent;
class ParryInstance;

class PlayerNode : public CharacterBody3D {
	GDCLASS(PlayerNode, CharacterBody3D)

public:
	PlayerFSM m_fsm;
	StateContext* m_stateContext = nullptr;
	CameraPivot* m_camerapivot = nullptr;
	Node3D* m_meshAnchor = nullptr;
	Area3D* m_grappleDetectionArea = nullptr;

	GrappleComponent* m_grappleComponent = nullptr;
	ParryInstigatorComponent* m_parryComponent = nullptr;

	std::vector<GrappleComponent*> m_inRangeGrapplenodes;

public:
	GETNAME(PlayerNode)
	static void _bind_methods();

	void _notification(int what);
	void _enter_tree() override;
	void _exit_tree() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;
	void _input(const Ref<InputEvent>& p_event);

	void rotateTowardsVelocity(float delta);

	void areaEnteredGrappledetection(Area3D* area);
	void areaExitedGrappledetection(Area3D* area);
	void determineGrappleTarget();
};

#endif // GD_CHARACTER_PLAYERNODE_PLUGIN_GAMEPLAY_H