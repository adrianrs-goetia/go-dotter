#include <character/playerStates.h>

#include <components/attackComponent.h>
#include <components/grappleInstigatorComponent.h>
#include <components/grappleTargetComponent.h>
#include <components/parryInstigatorComponent.h>
#include <components/dataObjects/parryInstance.hpp>

#include <managers/inputManager.h>

#include <configHandler.h>
#include <debugdraw3d/api.h>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>

#define CONFIG_PREFIX "player"

using namespace godot;

namespace helper {
void movement_acceleration(StateContext& context, float acceleration, float deceleration, float delta) {
	// direction
	if (context.input->m_inputRaw.abs() > Vector2()) {
		context.physics.velocity.x = Math::move_toward(context.physics.velocity.x,
				context.input->m_inputCameraRelative.x * GETPARAM_F("walkSpeed"), acceleration * delta);
		context.physics.velocity.z = Math::move_toward(context.physics.velocity.z,
				context.input->m_inputCameraRelative.y * GETPARAM_F("walkSpeed"), acceleration * delta);
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
		context.physics.velocity.y += GETPARAM_D("jumpStrength");
		return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
	}
	return {};
}

PlayerState::Return PlayerOnGroundState::physicsProcess(StateContext& context, float delta) {
	context.physics.velocity.y += (GETPARAMGLOBAL_D("gravityConstant") * GETPARAM_D("gravityScale")) * delta;

	// walking off edge
	if (!context.physics.isOnGround) {
		return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
	}

	return {};
}
PlayerState::Return PlayerOnGroundState::handleInput(StateContext& context, float delta) {
	// direction
	helper::movement_acceleration(
			context, GETPARAM_D("onGroundAcceleration"), GETPARAM_D("onGroundDeceleration"), delta);

	// actions
	if (context.input->isActionPressed(EInputAction::JUMP)) {
		context.physics.velocity.y += GETPARAM_D("jumpStrength");
		return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
	}
	if (context.input->isActionPressed(EInputAction::GRAPPLE) && context.grapple->getTarget()) {
		return Return{ PlayerStateBank::get().state<PlayerPreGrappleLaunchState>() };
	}
	if (context.input->isActionPressed(EInputAction::PARRY)) {
		return { PlayerStateBank::get().state<PlayerParryState>() };
	}
	if (context.input->isActionPressed(EInputAction::ATTACK)) {
		return { PlayerStateBank::get().state<PlayerAttackState>() };
	}
	return {};
}

// PlayerInAirState
PlayerState::Return PlayerInAirState::physicsProcess(StateContext& context, float delta) {
	if (context.physics.isOnGround) {
		// if (!m_guarantee_one_frame_processing)
		{
			DebugDraw::Position(Transform3D(Basis(), Vector3(context.physics.position)), Color(1, 1, 1), 2.f);
			return Return{ PlayerStateBank::get().state<PlayerOnGroundState>() };
		}
	}
	helper::movement_acceleration(context, GETPARAM_D("inAirAcceleration"), GETPARAM_D("inAirDeceleration"), delta);
	context.physics.velocity.y += (GETPARAMGLOBAL_D("gravityConstant") * GETPARAM_D("gravityScale")) * delta;
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
	GrappleTargetComponent::LaunchContext launch = context.grapple->launch(GETPARAM_D("grapple", "launchStrength"));
	if (launch.type != GrappleTargetComponent::LaunchType::INSTIGATOR_ANCHOR &&
			launch.type != GrappleTargetComponent::LaunchType::BOTH_ANCHOR) {
		context.physics.velocity = launch.impulse;
	}
	return Return{ PlayerStateBank::get().state<PlayerInAirState>() };
}

// PlayerParryState
bool PlayerParryState::canEnter() const {
	const bool canEnter = !m_exitTimestamp.timestampWithinTimeframe(GETPARAM_D("parry", "cooldown"));
	if (!canEnter) {
		LOG(DEBUG, "Tried entering parryState before cooldown was ready")
	}
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
	if (!m_enterTimestamp.timestampWithinTimeframe(GETPARAM_D("parry", "stateLength"))) {
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
		// Vector3 launchDir = pi->instigatorDesiredDirection * -1.f; // Flipped to towards input dir
		// const Vector3 orthoRight = g_up.cross(launchDir);
		// launchDir.rotate(orthoRight, Math::deg_to_rad(GETPARAM_D("parry", "launchUpAngle")) * -1.f);
		// context.physics.velocity = launchDir * GETPARAM_D("parry", "launchStrength");

		if (context.physics.isOnGround)
			return { PlayerStateBank::get().state<PlayerOnGroundState>() };
		else
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

// PlayerAttackState
bool PlayerAttackState::canEnter() const {
	const bool canEnter = !m_exitTimestamp.timestampWithinTimeframe(GETPARAM_D("attack", "cooldown"));
	if (!canEnter) {
		LOG(DEBUG, "Player AttackState still on cooldown")
	}
	return canEnter;
}

PlayerState::Return PlayerAttackState::enter(StateContext& context) {
	m_enterTimestamp.setTimestamp();
	context.attack->setComponentEnabled(true);
	context.attack->setAttackStrength(GETPARAM_F("attack", "strength"));
	// context.animation.playAnimation(EAnimation::ATTACK);
	return Return();
}

PlayerState::Return PlayerAttackState::exit(StateContext& context) {
	m_exitTimestamp.setTimestamp();
	context.attack->setComponentEnabled(false);
	return {};
}

PlayerState::Return PlayerAttackState::physicsProcess(StateContext& context, float delta) {
	DebugDraw::Position(
			Transform3D(Basis(), Vector3(context.physics.position + Vector3(0, 1, 0))), Color(0.7, 0, 0.5), delta);

	if (!m_enterTimestamp.timestampWithinTimeframe(GETPARAM_D("attack", "stateLength"))) {
		if (context.physics.isOnGround)
			return { PlayerStateBank::get().state<PlayerOnGroundState>() };
		else
			return { PlayerStateBank::get().state<PlayerInAirState>() };
	}

	context.physics.velocity *= 0;

	switch (context.attack->getAttackState()) {
		case AttackComponent::EState::HIT: {
			LOG(INFO, "HIT TARGET")
		}
		case AttackComponent::EState::NOT_HIT: {
		}

		default:
			break;
	}
	return {};
}
