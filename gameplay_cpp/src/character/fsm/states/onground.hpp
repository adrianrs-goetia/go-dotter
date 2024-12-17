#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <managers/inputManager.h>

#include <components/grappleInstigatorComponent.h>
#include <components/animation.hpp>
#include <components/parryInstigator.hpp>
#include <components/parryTarget.hpp>
#include <events/parry.hpp>

#define CONFIG_PREFIX "player"

namespace fsm::player {

class OnGroundState : public BaseState {
public:
	TState getType() const override {
		return TOnGroundState();
	};

	TState enter(Context& context) override {
		// Immediate jump when entering while having just pressed jump
		if (context.input->isActionPressed(EInputAction::JUMP, 0.1f)) {
			context.physics.velocity.y += GETPARAM_D("jumpStrength");
			return TInAirState();
		}
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) override {
		context.physics.velocity.y += (GETPARAMGLOBAL_D("gravityConstant") * GETPARAM_D("gravityScale")) * delta;

		// walking off edge
		if (!context.physics.isOnGround) {
			return TInAirState();
		}

		auto& vel = context.physics.velocity;
		const godot::Vector2 vel2d(vel.x, vel.z);
		const float speed = vel2d.length();
		float idleWalkBlend = godot::Math::clamp(speed / GETPARAM_F("walkSpeed"), 0.0f, 1.0f);
		context.anim->idleRunValue(idleWalkBlend);

		context.anim->rotateRootTowardsVector(
			context.input->getInputRelative3d(), delta, GETPARAM_D("animation", "rootRotationSpeed"));

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		// direction
		utils::movement_acceleration(
			context, GETPARAM_D("onGroundAcceleration"), GETPARAM_D("onGroundDeceleration"), delta);

		// actions
		if (context.input->isActionPressed(EInputAction::JUMP)) {
			if (auto lock = context.parry->m_lastParryContact.lock()) {
				lock->getTarget()->onAction(EventParryFreeze{ GETPARAM_F("parry", "freezetime") });
				return TParryJumpState();
			}

			context.physics.velocity.y += GETPARAM_D("jumpStrength");
			return TInAirState();
		}
		if (context.input->isActionPressed(EInputAction::GRAPPLE) && context.grapple->getTarget()) {
			return TPreGrappleLaunchState();
		}
		if (context.input->isActionPressed(EInputAction::PARRY)) {
			return TParryState();
		}
		if (context.input->isActionPressed(EInputAction::ATTACK)) {
			return TAttackState();
		}
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
