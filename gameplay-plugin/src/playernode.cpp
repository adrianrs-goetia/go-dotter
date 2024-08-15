#include <character/playernode.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>

extern "C" {
#include <stdlib.h>
}

void PlayerNode::_bind_methods() { DEFAULT_PROPERTY(PlayerNode) }

void PlayerNode::_enter_tree() {
	RETURN_IF_EDITOR
	m_state_context = (StateContext*)calloc(1, sizeof(StateContext));
	ASSERT(m_state_context != nullptr, "");
	m_state_context->physics.is_on_ground = is_on_floor();
	m_state_context->physics.position = get_position();
	m_state_context->physics.velocity = get_velocity();
	m_fsm.init(m_state_context);
	m_fsm.force_set_state<PlayerInAirState>(m_state_context);
}

void PlayerNode::_exit_tree() {
	RETURN_IF_EDITOR
	m_fsm.uninit();
	::free(m_state_context);
	m_state_context = nullptr;
	printf("PlayerNode exiting tree");
}

void PlayerNode::_process(float delta) { RETURN_IF_EDITOR }

void PlayerNode::_physics_process(float delta) {
	RETURN_IF_EDITOR
	ASSERT(m_state_context != nullptr, "");
	// capture current physics context
	m_state_context->physics.is_on_ground = is_on_floor();
	m_state_context->physics.position = get_position();
	m_state_context->physics.velocity = get_velocity();

	// Let FSM deal with physics and input context
	m_fsm.physics_process(delta);
	m_fsm.handle_input(delta);

	// set data from context
	set_velocity(m_state_context->physics.velocity);
	move_and_slide();

	// set position to 0
	auto v = get_position();
	set_position(Vector3(v.x, v.y, 0));

	// deferred actions
	m_fsm.deferred_actions();
}

void PlayerNode::_input(const Ref<InputEvent>& p_event) {
	RETURN_IF_EDITOR
	if (!m_state_context) {
		return;
	}
	m_state_context->input.input_direction = godot::Input::get_singleton()->get_vector(
			InputMap::move_left, InputMap::move_right, InputMap::ui_down, InputMap::ui_up, 0.05);
	if (p_event->is_action_pressed(InputMap::jump)) {
		m_state_context->input.input_action = InputAction{ EInputAction::JUMP, EInputActionType::PRESSED };
	}
	else {
		m_state_context->input.last_valid_input_action = m_state_context->input.input_action;
		m_state_context->input.input_action = InputAction{ EInputAction::NONE, EInputActionType::NONE };
	}
}