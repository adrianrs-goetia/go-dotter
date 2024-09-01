#include <character/playerstates.h>
#include <components/grapplecomponent.h>
#include <components/inputcomponent.h>

#include <debugdraw3d/api.h>
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
		if (context->input->input_raw.abs() > Vector2()) {
			context->physics.velocity.x = Math::move_toward(context->physics.velocity.x,
					context->input->input_relative.x * MAX_HORIZONTAL_SPEED, acceleration * delta);
			context->physics.velocity.z = Math::move_toward(context->physics.velocity.z,
					context->input->input_relative.y * MAX_HORIZONTAL_SPEED, acceleration * delta);
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

		const Vector3 impulse_dir = Vector3(context->input->input_relative.x, 1,
				context->input->input_relative.y)
											.normalized(); // TODO get better input movedir_rotated
		DebugDraw::Line(context->physics.position, context->physics.position + (impulse_dir * impulse_strength),
				Color(1, 0, 0), 2.f);
		context->physics.velocity = impulse_dir * impulse_strength;
	}
} //namespace helper

// PlayerOnGroundState
PlayerState::Return PlayerOnGroundState::enter(StateContext* context) {
	Super::enter(context);
	// Immediate jump when entering while having just pressed jump
	if (context->input->is_action_pressed(EInputAction::JUMP, 0.1f)) {
		context->physics.velocity.y += JUMP_STRENGTH;
		return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
	}
	return {};
}

PlayerState::Return PlayerOnGroundState::physics_process(StateContext* context, float delta) {
	context->physics.velocity.y -= GRAVITY * delta;

	// walking off edge
	if (!context->physics.is_on_ground) { return Return{ PlayerStateBank::get().state<PlayerInAirState>() }; }

	return {};
}
PlayerState::Return PlayerOnGroundState::handle_input(StateContext* context, float delta) {
	// direction
	helper::movement_acceleration(context, ONGROUND_ACCELERATION, ONGROUND_DECELARATION, delta);

	// actions
	if (context->input->is_action_pressed(EInputAction::JUMP)) {
		context->physics.velocity.y += JUMP_STRENGTH;
		return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
	}
	if (context->input->is_action_pressed(EInputAction::GRAPPLE) && context->grapple.target) {
		return Return{ PlayerStateBank::get().state<PlayerPreGrappleLaunchState>() };
	}
	return {};
}

// PlayerInAirState
PlayerState::Return PlayerInAirState::physics_process(StateContext* context, float delta) {
	if (context->physics.is_on_ground) {
		// if (!m_guarantee_one_frame_processing)
		{
			DebugDraw::Position(Transform3D(Basis(), Vector3(context->physics.position)), Color(1, 1, 1), 2.f);
			return Return{ PlayerStateBank::get().state<PlayerOnGroundState>() };
		}
	}
	helper::movement_acceleration(context, INAIR_ACCELERATION, INAIR_DECELARATION, delta);
	context->physics.velocity.y -= GRAVITY * delta;
	return {};
}

PlayerState::Return PlayerInAirState::handle_input(StateContext* context, float delta) {
	if (context->input->is_action_pressed(EInputAction::GRAPPLE) && context->grapple.target) {
		LOG(WARN, "inair grappling time")
		return Return{ PlayerStateBank::get().state<PlayerPreGrappleLaunchState>() };
	}
	if (context->input->is_action_pressed(EInputAction::PARRY)) {
		TypedArray<Vector3> close_positions =
				context->parry.get_parry_physics_query(context->physics.get_gravity_center());
		if (close_positions.size() > 0) { helper::parry_impulse(context, close_positions, PARRY_LAUNCH_STRENGTH); }
	}
	return {};
}

// PlayerPreGrappleLaunchState
PlayerState::Return PlayerPreGrappleLaunchState::enter(StateContext* context) {
	// TODO: PlayerState::can_enter_state() const ?? Make certain states not spammable?
	// TODO: camera adjustment and whatnot here?
	context->physics.velocity = Vector3();
	return Return{ PlayerStateBank::get().state<PlayerGrappleLaunchState>() };
}

// PlayerGrappleLaunchState
PlayerState::Return PlayerGrappleLaunchState::enter(StateContext* context) {
	// TODO... What to do here other than launch?
	GrappleComponent::LaunchContext launch = context->grapple.instigator->launch(context->grapple.target);
	if (launch.type != GrappleComponent::LaunchType::INSTIGATOR_ANCHOR &&
			launch.type != GrappleComponent::LaunchType::BOTH_ANCHOR) {
		context->physics.velocity = launch.impulse;
	}
	return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
}
