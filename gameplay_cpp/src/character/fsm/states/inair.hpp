#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <debugdraw3d/api.h>
#include <managers/inputManager.h>

#include <components/animation.hpp>
#include <components/grappleInstigator.hpp>

#ifdef CONFIG_PREFIX
#undef CONFIG_PREFIX
#endif
#define CONFIG_PREFIX "player"

namespace fsm::player {

class InAirState : public BaseState {
	TYPE(InAirState)

public:
	TState getType() const override {
		return TInAirState();
	}

	bool canEnter() const override {
		return true;
	}

	TState enter(Context& context) override {
		context.anim->inAir();
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) {
		if (context.physics.isOnGround) {
			DebugDraw::Position(Transform3D(Basis(), Vector3(context.physics.position)), Color(1, 1, 1), 2.f);
			return TOnGroundState();
		}
		utils::movementAcceleration(context, GETPARAM_D("inAirAcceleration"), GETPARAM_D("inAirDeceleration"), delta);
		// context.physics.velocity.y += (GETPARAMGLOBAL_D("gravityConstant") * GETPARAM_D("gravityScale")) * delta;

		context.anim->rotateRootTowardsVector(
			context.input->getInputRelative3d(), delta, GETPARAM_D("animation", "rootRotationSpeed"));

		return {};
	}

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) override {
		state->apply_central_force(
			Vector3(0, 1, 0) * (GETPARAMGLOBAL_D("gravityConstant") * GETPARAM_D("gravityScale")));
		// state->apply_central_force(context.physics.horizontalForce);
		return {};
	}

	TState handleInput(Context& context, float delta) override {
		if (context.input->isActionPressed(EInputAction::GRAPPLE) && context.grapple->getTarget()) {
			return TPreGrappleLaunchState();
		}
		if (context.input->isActionPressed(EInputAction::PARRY)) {
			return TParryPreState();
		}
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
