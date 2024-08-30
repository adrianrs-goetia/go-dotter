#include <character/playerstates.h>
#include <components/grapplecomponent.h>

#include <debugdraw3d/api.h>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/character_body3d.hpp>

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

constexpr float PARRY_LAUNCH_STRENGTH = 8.0f;

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

	// Expected to only handle collision queries containing at least a single point within the collison shape
	void parry_impulse(StateContext* context, const TypedArray<Vector3>& positions, float impulse_strength) {
		ASSERT(positions.size() > 0, "")

		const float debug_draw_duration = 0.2f;
		DebugDraw::Sphere(context->physics.get_gravity_center(), context->parry.detectionradius, Color(1.2, 0.2, 0.4),
				debug_draw_duration);

		Vector3 closest = positions[0];
		for (int i = 0; i < positions.size(); i++) {
			const Vector3 v3 = positions[i];
			if (v3.length_squared() < closest.length_squared()) { closest = v3; }
		}
		DebugDraw::Sphere(closest, 0.8f, Color(0, 2, 1, 0), debug_draw_duration);

		const Vector3 impulse_dir = Vector3(context->input.movedir_rotated.x, 1,
				context->input.movedir_rotated.y)
											.normalized(); // TODO get better input movedir_rotated
		DebugDraw::Line(context->physics.position, context->physics.position + (impulse_dir * impulse_strength),
				Color(1, 0, 0), 2.f);
		context->physics.velocity = impulse_dir * impulse_strength;
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
	return {};
}

// PlayerInAirState
StateReturn PlayerInAirState::physics_process(StateContext* context, float delta) {
	if (context->physics.is_on_ground) {
		if (!m_guarantee_one_frame_processing) {
			DebugDraw::Position(Transform3D(Basis(), Vector3(context->physics.position)), Color(1, 1, 1), 2.f);
			return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerOnGroundState>(false) };
		}
	}
	helper::movement_acceleration(context, INAIR_ACCELERATION, INAIR_DECELARATION, delta);
	context->physics.velocity.y -= GRAVITY * delta;
	return {};
}

StateReturn PlayerInAirState::handle_input(StateContext* context, float delta) {
	if (godot::Input::get_singleton()->is_action_just_pressed(InputMap::grapplehook) && context->grapple.target) {
		LOG(WARN, "inair grappling time")
		return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerPreGrappleLaunchState>(false) };
	}
	if (godot::Input::get_singleton()->is_action_just_pressed(InputMap::parry)) {
		TypedArray<Vector3> close_positions =
				context->parry.get_parry_physics_query(context->physics.get_gravity_center());
		if (close_positions.size() > 0) { helper::parry_impulse(context, close_positions, PARRY_LAUNCH_STRENGTH); }
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
	// const Vector3 launch_dir = Vector3(context->grapple.target_position - context->physics.position).normalized();
	// context->physics.velocity = launch_dir * GRAPPLE_LAUNCH_STRENGTH;
	context->grapple.instigator->launch(context->grapple.target);
	context->physics.velocity = context->grapple.instigator->get_parent_node<CharacterBody3D>()->get_velocity();
	return StateReturn{ EStateReturn::NEW_STATE, PlayerStateBank::get().state<PlayerInAirState>(false) };
}
