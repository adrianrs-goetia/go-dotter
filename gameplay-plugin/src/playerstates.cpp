#include <character/playerstates.h>

constexpr float MAX_HORIZONTAL_SPEED = 6.5f;
constexpr float ONGROUND_ACCELERATION = 40.0f;
constexpr float ONGROUND_DECELARATION = 30.0f;
constexpr float JUMP_STRENGTH = 9.0f;
constexpr float INAIR_ACCELERATION = 10.0f;
constexpr float INAIR_DECELARATION = 10.0f;

constexpr float GRAVITY_CONSTANT = 9.81f;
constexpr float GRAVITY_SCALE = 2.6f;
constexpr float GRAVITY = GRAVITY_CONSTANT * GRAVITY_SCALE;

namespace helper {
	void movement_acceleration(StateContext* context, float acceleration, float deceleration, float delta) {
		// direction
		if (context->input.movedir.abs() > Vector2()) {
			context->physics.velocity.x = Math::move_toward(context->physics.velocity.x,
					context->input.movedir_rotated.x * MAX_HORIZONTAL_SPEED, acceleration * delta);
			context->physics.velocity.z = Math::move_toward(context->physics.velocity.z,
					context->input.movedir_rotated.y * MAX_HORIZONTAL_SPEED, acceleration * delta);
		}
		else {
			context->physics.velocity.x = Math::move_toward(context->physics.velocity.x, 0.0f, deceleration * delta);
			context->physics.velocity.z = Math::move_toward(context->physics.velocity.z, 0.0f, deceleration * delta);
		}
	}
} //namespace helper

// PlayerOnGroundState
StateReturn PlayerOnGroundState::enter_state(StateContext* context) {
	Super::enter_state(context);
	// Immediate jump when entering while having just pressed jump
	if (context->input.last_valid_input_action.is_action_down(EInputAction::JUMP) &&
			context->input.last_valid_input_action.received_input_within_timeframe(0.1)) {
		context->physics.velocity.y += JUMP_STRENGTH;
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
	helper::movement_acceleration(context, ONGROUND_ACCELERATION, ONGROUND_DECELARATION, delta);

	// actions
	if (context->input.input_action.is_action_down(EInputAction::JUMP)) {
		context->physics.velocity.y += JUMP_STRENGTH;
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
	helper::movement_acceleration(context, INAIR_ACCELERATION, INAIR_DECELARATION, delta);
	context->physics.velocity.y -= GRAVITY * delta;
	return {};
}
