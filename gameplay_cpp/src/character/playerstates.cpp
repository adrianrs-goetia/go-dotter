#include <character/playerstates.h>
#include <components/grapplecomponent.h>
#include <components/inputcomponent.h>
#include <components/parryInstance.h>
#include <components/parryinstigatorcomponent.h>

#include <debugdraw3d/api.h>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>

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

constexpr float PARRY_STATE_COOLDOWN = 0.5f;
constexpr float PARRY_STATE_TIME_LENGTH = 0.7f;
constexpr float PARRY_STATE_LAUNCH_UP_ANGLE = 60.0f;
constexpr float PARRY_STATE_LAUNCH_STRENGTH = 10.0f;

namespace helper {
	void movement_acceleration(StateContext& context, float acceleration, float deceleration, float delta) {
		// direction
		if (context.input->m_inputRaw.abs() > Vector2()) {
			context.physics.velocity.x = Math::move_toward(context.physics.velocity.x,
					context.input->m_inputCameraRelative.x * MAX_HORIZONTAL_SPEED, acceleration * delta);
			context.physics.velocity.z = Math::move_toward(context.physics.velocity.z,
					context.input->m_inputCameraRelative.y * MAX_HORIZONTAL_SPEED, acceleration * delta);
		}
		else {
			context.physics.velocity.x = Math::move_toward(context.physics.velocity.x, 0.0f, deceleration * delta);
			context.physics.velocity.z = Math::move_toward(context.physics.velocity.z, 0.0f, deceleration * delta);
		}
	}
} //namespace helper

// PlayerOnGroundState
PlayerState::Return PlayerOnGroundState::enter(StateContext& context) {
	Super::enter(context);
	// Immediate jump when entering while having just pressed jump
	if (context.input->isActionPressed(EInputAction::JUMP, 0.1f)) {
		context.physics.velocity.y += JUMP_STRENGTH;
		return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
	}
	return {};
}

PlayerState::Return PlayerOnGroundState::physicsProcess(StateContext& context, float delta) {
	context.physics.velocity.y -= GRAVITY * delta;

	// walking off edge
	if (!context.physics.is_on_ground) { return Return{ PlayerStateBank::get().state<PlayerInAirState>() }; }

	return {};
}
PlayerState::Return PlayerOnGroundState::handleInput(StateContext& context, float delta) {
	// direction
	helper::movement_acceleration(context, ONGROUND_ACCELERATION, ONGROUND_DECELARATION, delta);

	// actions
	if (context.input->isActionPressed(EInputAction::JUMP)) {
		context.physics.velocity.y += JUMP_STRENGTH;
		return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
	}
	if (context.input->isActionPressed(EInputAction::GRAPPLE) && context.grapple.target) {
		return Return{ PlayerStateBank::get().state<PlayerPreGrappleLaunchState>() };
	}
	if (context.input->isActionPressed(EInputAction::PARRY)) {
		return { PlayerStateBank::get().state<PlayerParryState>() };
	}
	return {};
}

// PlayerInAirState
PlayerState::Return PlayerInAirState::physicsProcess(StateContext& context, float delta) {
	if (context.physics.is_on_ground) {
		// if (!m_guarantee_one_frame_processing)
		{
			DebugDraw::Position(Transform3D(Basis(), Vector3(context.physics.position)), Color(1, 1, 1), 2.f);
			return Return{ PlayerStateBank::get().state<PlayerOnGroundState>() };
		}
	}
	helper::movement_acceleration(context, INAIR_ACCELERATION, INAIR_DECELARATION, delta);
	context.physics.velocity.y -= GRAVITY * delta;
	return {};
}

PlayerState::Return PlayerInAirState::handleInput(StateContext& context, float delta) {
	if (context.input->isActionPressed(EInputAction::GRAPPLE) && context.grapple.target) {
		LOG(WARN, "inair grappling time")
		return Return{ PlayerStateBank::get().state<PlayerPreGrappleLaunchState>() };
	}
	if (context.input->isActionPressed(EInputAction::PARRY)) {
		return { PlayerStateBank::get().state<PlayerParryState>() };
	}
	return {};
}

// PlayerPreGrappleLaunchState
PlayerState::Return PlayerPreGrappleLaunchState::enter(StateContext& context) {
	// TODO: PlayerState::can_enter_state() const ?? Make certain states not spammable?
	// TODO: camera adjustment and whatnot here?
	context.physics.velocity = Vector3();
	return Return{ PlayerStateBank::get().state<PlayerGrappleLaunchState>() };
}

// PlayerGrappleLaunchState
PlayerState::Return PlayerGrappleLaunchState::enter(StateContext& context) {
	// TODO... What to do here other than launch?
	GrappleComponent::LaunchContext launch = context.grapple.instigator->launch(context.grapple.target);
	if (launch.type != GrappleComponent::LaunchType::INSTIGATOR_ANCHOR &&
			launch.type != GrappleComponent::LaunchType::BOTH_ANCHOR) {
		context.physics.velocity = launch.impulse;
	}
	return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
}

// PlayerParryState
bool PlayerParryState::canEnter() const {
	const bool canEnter = !m_exitTimestamp.timestampWithinTimeframe(PARRY_STATE_COOLDOWN);
	if (!canEnter) { LOG(DEBUG, "Tried entering parryState before cooldown was ready") }
	LOG(INFO, "Player parrying")
	return canEnter;
}

PlayerState::Return PlayerParryState::enter(StateContext& context) {
	Super::enter(context);
	m_enterTimestamp.setTimestamp();
	context.physics.velocity = Vector3(); // zero out velocity while in
	return {};
}

PlayerState::Return PlayerParryState::exit(StateContext& context) {
	m_exitTimestamp.setTimestamp();
	return {};
}

PlayerState::Return PlayerParryState::physicsProcess(StateContext& context, float delta) {
	// Parry state timed out
	if (!m_enterTimestamp.timestampWithinTimeframe(PARRY_STATE_TIME_LENGTH)) {
		// Enter on ground by default, should discern if in air or onGround?
		return { PlayerStateBank::get().state<PlayerOnGroundState>() };
	}
	if (const auto pi = context.parry->activateParry()) {
		// Play effects
		context.audioVisual.audio->play();
		context.audioVisual.particles->set_global_position(pi->targetPosition);
		context.audioVisual.particles->set_global_basis(getBasisTowardsDirection(pi->instigatorDesiredDirection));
		context.audioVisual.particles->restart();

		// Launch player
		Vector3 launchDir = pi->instigatorDesiredDirection * -1.f; // Flipped to towards input dir
		const Vector3 orthoRight = g_up.cross(launchDir);
		launchDir.rotate(orthoRight, Math::deg_to_rad(PARRY_STATE_LAUNCH_UP_ANGLE) * -1.f);
		context.physics.velocity = launchDir * PARRY_STATE_LAUNCH_STRENGTH;

		return { PlayerStateBank::get().state<PlayerInAirState>() };
	}
	return {};
}

PlayerState::Return PlayerParryState::handleInput(StateContext& context, float delta) {
	if (context.input->isActionPressed(EInputAction::JUMP)) {
		return { PlayerStateBank::get().state<PlayerInAirState>() };
	}
	return {};
}
