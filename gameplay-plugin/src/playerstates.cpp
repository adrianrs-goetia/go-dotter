#include <character/playerstates.h>

constexpr float MAX_HORIZONTAL_SPEED = 6.5f;
constexpr float ONGROUND_ACCELERATION = 40.0f;
constexpr float ONGROUND_DECELARTION = 30.0f;

constexpr float GRAVITY = 9.81f;

// PlayerOnGroundState
StateReturn PlayerOnGroundState::enter_state(StateContext* context) {
	Super::enter_state(context);
	// Immediate jump when entering while having just pressed jump
	if (context->input.last_valid_input_action.is_action_down(EInputAction::JUMP) &&
			context->input.last_valid_input_action.received_input_within_timeframe(0.1)) {
		context->physics.velocity.y += 8.0f;
		return StateReturn{ EStateReturn::NEW_STATE, new PlayerInAirState(true) };
	}
	return {};
}

StateReturn PlayerOnGroundState::physics_process(StateContext* context, float delta) {
	context->physics.velocity.y -= GRAVITY * delta;

	// walking off edge
	if (!context->physics.is_on_ground) {
		return StateReturn{ EStateReturn::NEW_STATE, new PlayerInAirState(false) };
	}
	return {};
}
StateReturn PlayerOnGroundState::handle_input(StateContext* context, float delta) {
	// direction
	if (context->input.input_direction.abs() > Vector2()) {
		context->physics.velocity.x = Math::move_toward(context->physics.velocity.x,
				context->input.input_direction.x * MAX_HORIZONTAL_SPEED, ONGROUND_ACCELERATION * delta);
		context->physics.velocity.z = Math::move_toward(context->physics.velocity.z,
				context->input.input_direction.y * MAX_HORIZONTAL_SPEED, ONGROUND_ACCELERATION * delta);
	}
	else {
		context->physics.velocity.x =
				Math::move_toward(context->physics.velocity.x, 0.0f, ONGROUND_DECELARTION * delta);
		context->physics.velocity.z =
				Math::move_toward(context->physics.velocity.z, 0.0f, ONGROUND_DECELARTION * delta);
	}

	// actions
	if (context->input.input_action.is_action_down(EInputAction::JUMP)) {
		context->physics.velocity.y += 8.0f;
		return StateReturn{ EStateReturn::NEW_STATE, new PlayerInAirState(false) };
	}
	return {};
}

// PlayerInAirState
StateReturn PlayerInAirState::physics_process(StateContext* context, float delta) {
	if (context->physics.is_on_ground) {
		if (!m_guarantee_one_frame) {
			return StateReturn{ EStateReturn::NEW_STATE, new PlayerOnGroundState(false) };
		}
	}
	context->physics.velocity.y -= GRAVITY * delta;
	return {};
}
