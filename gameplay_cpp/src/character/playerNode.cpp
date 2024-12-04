#include <character/cameraPivot.h>
#include <character/playerNode.h>
#include <components/grappleInstigatorComponent.h>
#include <components/grappleTargetComponent.h>
#include <components/parryInstigatorComponent.h>
#include <managers/inputManager.h>

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_action.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <configHandler.h>
#include <debugdraw3d/api.h>

extern "C" {
#include <stdlib.h>
}

using namespace godot;

void PlayerNode::_bind_methods() {}

void PlayerNode::_notification(int what) {
	// LOG(INFO, "Notification", (int64_t)what)
	switch (what) {
		case NOTIFICATION_WM_CLOSE_REQUEST: {
			LOG(INFO, "PlayerNode WM_CLOSE_REQUEST")
			// exit_game();
			break;
		}

		default: break;
	}
}

void PlayerNode::_enter_tree() {
	Log(ELog::DEBUG, "PlayerNode entering tree -- editor");

	RETURN_IF_EDITOR(void())

	auto* input = InputManager::get(*this);
	m_parryComponent = getChildOfNode<ParryInstigatorComponent>(this);
	auto* grappleInstigator = getChildOfNode<GrappleInstigatorComponent>(this);
	auto* audio = getChildOfNode<AudioStreamPlayer3D>(this);
	auto* particles = getChildOfNode<GPUParticles3D>(this);

	Log(ELog::DEBUG, "PlayerNode entering tree");

	m_stateContext = (StateContext*)calloc(1, sizeof(StateContext));
	m_meshAnchor = get_node<Node3D>("meshAnchor");
	m_camerapivot = get_node<CameraPivot>(nodePaths::cameraPivot);

	ASSERT_NOTNULL(input)
	ASSERT_NOTNULL(m_parryComponent)
	ASSERT_NOTNULL(m_stateContext)
	ASSERT_NOTNULL(m_camerapivot)
	ASSERT_NOTNULL(m_meshAnchor)
	ASSERT_NOTNULL(grappleInstigator)
	ASSERT_NOTNULL(audio)
	ASSERT_NOTNULL(particles)

	m_stateContext->input = input;
	m_stateContext->parry = m_parryComponent;
	m_stateContext->physics.is_on_ground = is_on_floor();
	m_stateContext->physics.position = get_position();
	m_stateContext->physics.velocity = get_velocity();
	m_stateContext->audioVisual.audio = audio;
	m_stateContext->audioVisual.particles = particles;
	m_fsm.init(*m_stateContext, PlayerStateBank::get().state<PlayerInAirState>());

	grappleInstigator->setInstigatorDirection(
			[](const Node& node) -> Vector3 { return InputManager::get(node)->getCamera3dDir(); });
	m_stateContext->grapple = grappleInstigator;

	m_parryComponent->m_getDesiredDirectionCb = [this]() -> Vector3 {
		Vector3 desiredDir = m_stateContext->input->getInputRelative3d();
		if (desiredDir.length_squared() < 0.2f) { desiredDir = m_stateContext->input->getCamera3dDir(); }
		// desiredDir is flipped to launch player and object in the opposite directions
		return desiredDir * -1.f;
	};
}

void PlayerNode::_exit_tree() {
	RETURN_IF_EDITOR(void())

	Log(ELog::DEBUG, "PlayerNode exiting tree");

	ASSERT_NOTNULL(m_stateContext);

	::free(m_stateContext);
	m_stateContext = nullptr;
}

void PlayerNode::_process(double delta) {
	RETURN_IF_EDITOR(void())

	ASSERT_NOTNULL(m_camerapivot);
	m_camerapivot->process(*m_stateContext, delta);
}

void PlayerNode::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	ASSERT_NOTNULL(m_stateContext);
	// capture current physics context
	m_stateContext->physics.is_on_ground = is_on_floor();
	m_stateContext->physics.position = get_position();
	m_stateContext->physics.velocity = get_velocity();

	// Let FSM deal with physics and input context
	m_fsm.physicsProcess(*m_stateContext, delta);
	m_fsm.handleInput(*m_stateContext, delta);

	// set data from context
	set_velocity(m_stateContext->physics.velocity);
	move_and_slide();
	rotateTowardsVelocity(
			m_stateContext->input->getInputRelative3d(), delta, GETPARAM_D("player", "animation", "rootRotationSpeed"));

	// deferred actions
	m_fsm.deferredActions(*m_stateContext);
}

void PlayerNode::_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR(void())

	if (!m_stateContext) { return; }
	ASSERT_NOTNULL(m_camerapivot);
	m_camerapivot->processInput(*m_stateContext, get_process_delta_time());
}

void PlayerNode::rotateTowardsVelocity(Vector3 inputvec, float delta, float slerpWeight) {
	if (inputvec.length_squared() <= 0) { return; }
	inputvec.normalize();

	float angle = Math::acos(g_forward.dot(inputvec));
	const int angle_dir = (g_right.dot(inputvec) > 0.f) ? 1 : -1;
	angle *= angle_dir;
	const Quaternion curquat = m_meshAnchor->get_transform().get_basis().get_quaternion();
	const Quaternion targetquat(g_up, angle);
	Quaternion newquat = curquat.slerp(targetquat, delta * slerpWeight);
	m_meshAnchor->set_basis(Basis(newquat));
}
