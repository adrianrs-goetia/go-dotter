#include <character/camerapivot.h>
#include <character/playernode.h>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/viewport.hpp>

extern "C" {
#include <stdlib.h>
}

using namespace godot;

void PlayerNode::_bind_methods() { DEFAULT_PROPERTY(PlayerNode) }

void PlayerNode::_enter_tree() {
	RETURN_IF_EDITOR

	Log(ELog::DEBUG, "PlayerNode entering tree");
	m_state_context = (StateContext*)calloc(1, sizeof(StateContext));
	ASSERT(m_state_context != nullptr, "");

	m_camerapivot = get_node<CameraPivot>(NodePaths::camera_pivot);
	ASSERT(m_camerapivot != nullptr, "");

	m_state_context->physics.is_on_ground = is_on_floor();
	m_state_context->physics.position = get_position();
	m_state_context->physics.velocity = get_velocity();
	m_fsm.force_set_state<PlayerInAirState>(m_state_context);
}

void PlayerNode::_exit_tree() {
	RETURN_IF_EDITOR
	Log(ELog::DEBUG, "PlayerNode exiting tree");
	::free(m_state_context);
	m_state_context = nullptr;
}

void PlayerNode::_process(float delta) {
	RETURN_IF_EDITOR
	ASSERT(m_camerapivot != nullptr, "");
	m_camerapivot->process(m_state_context, delta);
}

void PlayerNode::_physics_process(float delta) {
	RETURN_IF_EDITOR
	ASSERT(m_state_context != nullptr, "");
	// capture current physics context
	m_state_context->physics.is_on_ground = is_on_floor();
	m_state_context->physics.position = get_position();
	m_state_context->physics.velocity = get_velocity();

	// Let FSM deal with physics and input context
	m_fsm.physics_process(m_state_context, delta);
	m_fsm.handle_input(m_state_context, delta);

	// set data from context
	set_velocity(m_state_context->physics.velocity);
	move_and_slide();

	// deferred actions
	m_fsm.deferred_actions(m_state_context);
}

void PlayerNode::_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR
	if (!m_state_context) {
		return;
	}

	const float delta = get_process_delta_time();
	Input* input = Input::get_singleton();

	m_state_context->input.movedir = input->get_vector(
			InputMap::move_left, InputMap::move_right, InputMap::move_forward, InputMap::move_backward, 0.05);
	if (p_event->is_action_pressed(InputMap::jump)) {
		m_state_context->input.input_action = InputAction{ EInputAction::JUMP, EInputActionType::PRESSED };
	}
	else {
		m_state_context->input.last_valid_input_action = m_state_context->input.input_action;
		m_state_context->input.input_action = InputAction{ EInputAction::NONE, EInputActionType::NONE };
	}

	ASSERT(m_camerapivot != nullptr, "");
	m_camerapivot->process_input(m_state_context, input, p_event, delta);
}