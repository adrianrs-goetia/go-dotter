#include <character/playerstates.h>

#include <godot_cpp/classes/input.hpp>

constexpr float MAX_HORIZONTAL_SPEED = 6.5f;
constexpr float ONGROUND_ACCELERATION = 40.0f;
constexpr float ONGROUND_DECELARATION = 30.0f;
constexpr float JUMP_STRENGTH = 9.0f;
constexpr float INAIR_ACCELERATION = 10.0f;
constexpr float INAIR_DECELARATION = 10.0f;

constexpr float GRAVITY_CONSTANT = 9.81f;
constexpr float GRAVITY_SCALE = 2.6f;
constexpr float GRAVITY = GRAVITY_CONSTANT * GRAVITY_SCALE;

constexpr float GRAPPLE_LAUNCH_STRENGTH = 20.0f;

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
		return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerInAirState>(true) };
	}
	return {};
}

StateReturn PlayerOnGroundState::physics_process(StateContext* context, float delta) {
	context->physics.velocity.y -= GRAVITY * delta;

	// walking off edge
	if (!context->physics.is_on_ground) {
		return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerInAirState>(false) };
	}
	DEBUG_DRAW_BOX(Vector3(0, 1, 0), Quaternion(1, 0, 0, 0), Vector3(2, 2, 2), Color(1, 0, 0));

	return {};
}
StateReturn PlayerOnGroundState::handle_input(StateContext* context, float delta) {
	// direction
	helper::movement_acceleration(context, ONGROUND_ACCELERATION, ONGROUND_DECELARATION, delta);

	// actions
	// if (context->input.input_action.is_action_down(EInputAction::JUMP)) { ## TODO improve capturing input. Preferably
	// avoid this at state level
	if (godot::Input::get_singleton()->is_action_pressed(InputMap::jump)) {
		context->physics.velocity.y += JUMP_STRENGTH;
		return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerInAirState>(false) };
	}
	else if (godot::Input::get_singleton()->is_action_just_pressed(InputMap::grapplehook) && context->grapple.target) {
		return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerPreGrappleLaunchState>(false) };
	}
	else if (godot::Input::get_singleton()->is_action_just_pressed(InputMap::parry)) {
		DEBUG_DRAW_SPHERE(
				context->physics.get_gravity_center(), context->parry.detectionradius, Color(0.2, 0.9, 0.1), 2.f);
	}
	return {};
}

// PlayerInAirState
StateReturn PlayerInAirState::physics_process(StateContext* context, float delta) {
	if (context->physics.is_on_ground) {
		if (!m_guarantee_one_frame_processing) {
			DEBUG_DRAW_POSITION(Transform3D(Basis(), Vector3(context->physics.position)), Color(1, 1, 1), 2.f);
			return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerOnGroundState>(false) };
		}
	}
	helper::movement_acceleration(context, INAIR_ACCELERATION, INAIR_DECELARATION, delta);
	context->physics.velocity.y -= GRAVITY * delta;
	return {};
}

StateReturn PlayerInAirState::handle_input(StateContext* context, float delta) {
	if (godot::Input::get_singleton()->is_action_just_pressed(InputMap::grapplehook) && context->grapple.target) {
		return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerPreGrappleLaunchState>(false) };
	}

	return StateReturn();
}

// PlayerPreGrappleLaunchState
StateReturn PlayerPreGrappleLaunchState::enter_state(StateContext* context) {
	// TODO: PlayerState::can_enter_state() const ?? Make certain states not spammable?
	// TODO: camera adjustment and whatnot here?
	context->physics.velocity = Vector3();
	return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerGrappleLaunchState>(false) };
}

// PlayerGrappleLaunchState
StateReturn PlayerGrappleLaunchState::enter_state(StateContext* context) {
	// TODO... What to do here other than launch?
	const Vector3 launch_dir = Vector3(context->grapple.target_position - context->physics.position).normalized();
	context->physics.velocity = launch_dir * GRAPPLE_LAUNCH_STRENGTH;
	return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerInAirState>(false) };
}
