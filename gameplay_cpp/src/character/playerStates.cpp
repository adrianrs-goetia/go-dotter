#include <character/playerStates.h>
#include <components/grappleInstigatorComponent.h>
#include <components/grappleTargetComponent.h>
#include <components/parryInstigatorComponent.h>
#include <managers/inputManager.h>
#include <components/dataObjects/parryInstance.hpp>

#include <configHandler.h>
#include <debugdraw3d/api.h>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>

using namespace godot;

namespace helper {
void movement_acceleration(StateContext& context, float acceleration, float deceleration, float delta) {
	// direction
	if (context.input->m_inputRaw.abs() > Vector2()) {
		context.physics.velocity.x = Math::move_toward(context.physics.velocity.x,
				context.input->m_inputCameraRelative.x * GETPARAM_F("player", "walkSpeed"), acceleration * delta);
		context.physics.velocity.z = Math::move_toward(context.physics.velocity.z,
				context.input->m_inputCameraRelative.y * GETPARAM_F("player", "walkSpeed"), acceleration * delta);
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
		context.physics.velocity.y += GETPARAM_D("player", "jumpStrength");
		return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
	}
	return {};
}

PlayerState::Return PlayerOnGroundState::physicsProcess(StateContext& context, float delta) {
	context.physics.velocity.y += (GETPARAM_D("gravityConstant") * GETPARAM_D("player", "gravityScale")) * delta;

	// walking off edge
	if (!context.physics.is_on_ground) { return Return{ PlayerStateBank::get().state<PlayerInAirState>() }; }

	return {};
}
PlayerState::Return PlayerOnGroundState::handleInput(StateContext& context, float delta) {
	// direction
	helper::movement_acceleration(context, GETPARAM_D("player", "onGroundAcceleration"), GETPARAM_D("player", "onGroundDeceleration"), delta);

	// actions
	if (context.input->isActionPressed(EInputAction::JUMP)) {
		context.physics.velocity.y += GETPARAM_D("player", "jumpStrength");
		return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
	}
	if (context.input->isActionPressed(EInputAction::GRAPPLE) && context.grapple->getTarget()) {
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
	helper::movement_acceleration(context, GETPARAM_D("player", "inAirAcceleration"), GETPARAM_D("player", "inAirDeceleration"), delta);
	context.physics.velocity.y += (GETPARAM_D("gravityConstant") * GETPARAM_D("player", "gravityScale")) * delta;
	return {};
}

PlayerState::Return PlayerInAirState::handleInput(StateContext& context, float delta) {
	if (context.input->isActionPressed(EInputAction::GRAPPLE) && context.grapple->getTarget()) {
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
	GrappleTargetComponent::LaunchContext launch =
			context.grapple->launch(GETPARAM_D("player", "grapple", "launchStrength"));
	if (launch.type != GrappleTargetComponent::LaunchType::INSTIGATOR_ANCHOR &&
			launch.type != GrappleTargetComponent::LaunchType::BOTH_ANCHOR) {
		context.physics.velocity = launch.impulse;
	}
	return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
}

// PlayerParryState
bool PlayerParryState::canEnter() const {
	const bool canEnter = !m_exitTimestamp.timestampWithinTimeframe(GETPARAM_D("player", "parry", "cooldown"));
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
	if (!m_enterTimestamp.timestampWithinTimeframe(GETPARAM_D("player", "parry", "stateLength"))) {
		// Enter on ground by default, should discern if in air or onGround?
		return { PlayerStateBank::get().state<PlayerOnGroundState>() };
	}
	if (const auto pi = context.parry->activateParry()) {
		// Play effects
		context.audioVisual.audio->play();
		context.audioVisual.particles->set_global_position(pi->targetPosition);
		context.audioVisual.particles->set_global_basis(createBasisFromDirection(pi->instigatorDesiredDirection));
		context.audioVisual.particles->restart();

		// Launch player
		Vector3 launchDir = pi->instigatorDesiredDirection * -1.f; // Flipped to towards input dir
		const Vector3 orthoRight = g_up.cross(launchDir);
		launchDir.rotate(orthoRight, Math::deg_to_rad(GETPARAM_D("player", "parry", "launchUpAngle")) * -1.f);
		context.physics.velocity = launchDir * GETPARAM_D("player", "parry", "launchStrength");

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
