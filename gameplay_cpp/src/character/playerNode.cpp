#include "playerNode.h"

#include <character/cameraPivot.h>
#include <character/fsm/fsm.hpp>
#include <character/gui/playerGui.hpp>

#include <managers/inputManager.h>
#include <components/animation.hpp>
#include <components/attackInstigator.hpp>
#include <components/grappleInstigator.hpp>
#include <components/grappleTarget.hpp>
#include <components/parryInstigator.hpp>

#include "../utils/circularbuffer.h"
#include "fsm/typedefs.hpp"
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_action.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/physics_material.hpp>
#include <godot_cpp/classes/progress_bar.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <debugdraw3d/api.h>
#include <configparams.hpp>

extern "C" {
#include <stdlib.h>
}

using namespace godot;

Transform3D PlayerNode::startTransform = {};

void PlayerNode::_bind_methods() {}

void PlayerNode::_notification(int what) {
	switch (what) {
		case NOTIFICATION_WM_CLOSE_REQUEST: {
			LOG(INFO, "PlayerNode WM_CLOSE_REQUEST")
			break;
		}
		case ENotifications::SOFT_RESTART: {
			set_global_transform(PlayerNode::startTransform);
			set_linear_velocity(Vector3());
		}

		default:
			break;
	}
}

void PlayerNode::_enter_tree() {
	set_lock_rotation_enabled(true);
	set_gravity_scale(0.0);
	set_contact_monitor(true); // Required for _integrate_forces()
	set_max_contacts_reported(4);
	set_can_sleep(false); // _integrate_forces is not invoked once body is sleeping
	{ // Each state needs to handle the physics material, so we override it here
		Ref<PhysicsMaterial> mat;
		mat.instantiate();
		set_physics_material_override(mat);
	}

	RETURN_IF_EDITOR(void())

	PlayerNode::startTransform = get_global_transform();

	auto* input = InputManager::get(*this);
	m_animComponent = getComponentOfNode<ComponentAnimation>(this);
	m_parryComponent = getComponentOfNode<ComponentParryInstigator>(this);
	auto* attackComponent = getComponentOfNode<ComponentAttackInstigator>(this);
	auto* grappleInstigator = getComponentOfNode<ComponentGrappleInstigator>(this);
	auto* audio = getComponentOfNode<AudioStreamPlayer3D>(this);
	auto* particles = getComponentOfNode<GPUParticles3D>(this);
	auto* states = new CircularBuffer<fsm::player::TState>{ 20, fsm::player::TOnGroundState{} };
	auto* gui = getComponentOfNode<PlayerGui>(this);

	fsm::player::Context stateContext;
	m_camerapivot = get_node<CameraPivot>(nodePaths::cameraPivot);

	ASSERTNN(input)
	ASSERTNN(m_parryComponent)
	ASSERTNN(m_camerapivot)
	ASSERTNN(m_animComponent)
	ASSERTNN(attackComponent)
	ASSERTNN(grappleInstigator)
	ASSERTNN(audio)
	ASSERTNN(particles)
	ASSERTNN(gui)

	stateContext.owner = this;
	stateContext.attack = attackComponent;
	stateContext.anim = m_animComponent;
	stateContext.input = input;
	stateContext.parry = m_parryComponent;
	stateContext.physics.position = get_position();
	stateContext.audioVisual.audio = audio;
	stateContext.audioVisual.particles = particles;
	stateContext.states = states;
	stateContext.gui = gui;

	grappleInstigator->setInstigatorDirection(
		[](const Node& node) -> Vector3 { return InputManager::get(node)->getCamera3dDir(); });
	stateContext.grapple = grappleInstigator;

	m_fsm = new fsm::player::Fsm(stateContext);
	ASSERTNN(m_fsm)
	m_fsm->init(fsm::player::TInAirState());
}

void PlayerNode::_exit_tree() {
	RETURN_IF_EDITOR(void())

	LOG(DEBUG, "PlayerNode exiting tree");

	delete m_fsm;
	m_fsm = nullptr;
}

void PlayerNode::_process(double delta) {
	RETURN_IF_EDITOR(void())

	ASSERTNN(m_camerapivot);
	m_camerapivot->process(m_fsm->getContext(), delta);
}

void PlayerNode::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	ASSERTNN(m_fsm)
	// capture current physics context
	auto& stateContext = m_fsm->getContext();
	stateContext.physics.position = get_position();

	// Let FSM deal with physics and input context
	m_fsm->handleInput(delta);
}

void PlayerNode::_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR(void())

	if (!m_fsm) {
		return;
	}
	ASSERTNN(m_camerapivot);
	m_camerapivot->processInput(m_fsm->getContext(), get_process_delta_time());
}

void PlayerNode::_integrate_forces(PhysicsDirectBodyState3D* state) {
	ASSERTNN(m_fsm)
	m_fsm->integrateForces(state);
}
