#include <character/playerstates.h>

constexpr float MAX_HORIZONTAL_SPEED = 6.5f;
constexpr float ONGROUND_ACCELERATION = 40.0f;
constexpr float ONGROUND_DECELARTION = 30.0f;

constexpr float GRAVITY = 9.81f;

// PlayerOnGroundIdleState
StateReturn PlayerOnGroundState::enter_state() {
	Super::enter_state();
	// Immediate jump when entering while having just pressed jump
	if (m_context->input.last_valid_input_action.is_action_down(EInputAction::JUMP) &&
			m_context->input.last_valid_input_action.received_input_within_timeframe(0.1)) {
		m_context->physics.velocity.y += 8.0f;
		return StateReturn{ EStateReturn::NEW_STATE, new PlayerInAirState(this, true) };
	}
	return {};
}

StateReturn PlayerOnGroundState::physics_process(real_t delta) {
	m_context->physics.velocity.y -= GRAVITY * delta;

	// walking off edge
	if (!m_context->physics.is_on_ground) {
		return StateReturn{ EStateReturn::NEW_STATE, new PlayerInAirState(this, false) };
	}
	return {};
}
StateReturn PlayerOnGroundState::handle_input(real_t delta) {
	// direction
	if (m_context->input.input_direction.abs().x) {
		m_context->physics.velocity.x = Math::move_toward(m_context->physics.velocity.x,
				m_context->input.input_direction.x * MAX_HORIZONTAL_SPEED, ONGROUND_ACCELERATION * delta);
	}
	else {
		m_context->physics.velocity.x =
				Math::move_toward(m_context->physics.velocity.x, 0.0f, ONGROUND_DECELARTION * delta);
	}
	// actions
	if (m_context->input.input_action.is_action_down(EInputAction::JUMP)) {
		m_context->physics.velocity.y += 8.0f;
		return StateReturn{ EStateReturn::NEW_STATE, new PlayerInAirState(this, false) };
	}
	return {};
}

// PlayerInAirState
StateReturn PlayerInAirState::physics_process(real_t delta) {
	if (m_context->physics.is_on_ground) {
		if (!m_guarantee_one_frame) {
			return StateReturn{ EStateReturn::NEW_STATE, new PlayerOnGroundState(this, false) };
		}
	}
	m_context->physics.velocity.y -= GRAVITY * delta;
	return {};
}
