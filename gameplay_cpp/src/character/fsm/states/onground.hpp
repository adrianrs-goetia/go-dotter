#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <managers/inputManager.h>

#include <components/animation.hpp>
#include <components/grappleInstigator.hpp>
#include <components/parryInstigator.hpp>
#include <components/parryTarget.hpp>
#include <events/parry.hpp>

#ifdef CONFIG_PREFIX
#undef CONFIG_PREFIX
#endif
#define CONFIG_PREFIX "player"

namespace fsm::player {

class OnGroundState : public BaseState {
	TYPE(OnGroundState)

public:
	TState getType() const override {
		return TOnGroundState();
	};

	TState enter(Context& context) override {
		context.anim->onGround();
		// Immediate jump when entering while having just pressed jump
		// if (context.input->isActionPressed(EInputAction::JUMP, 0.1f)) {
		// 	context.physics.velocity.y += GETPARAM_D("jumpStrength");
		// 	return TInAirState();
		// }
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) override {
		// walking off edge
		if (!context.physics.isOnGround) {
			return TInAirState();
		}

		// @todo, part of integrateForces..
		auto& vel = context.physics.velocity;
		const godot::Vector2 vel2d(vel.x, vel.z);
		const float speed = vel2d.length();
		
		float walkSpeed = GETPARAM_F("walkSpeed");
		float sprintSpeed = GETPARAM_F("sprintSpeed");
		float idleWalkBlend = godot::Math::clamp(speed / walkSpeed, 0.0f, 1.0f);
		float sprintBlend = godot::Math::clamp((speed-walkSpeed)/(sprintSpeed-walkSpeed), 0.0f, 1.0f); // =0 for walkSpeed, 1 for sprintSpeed
		context.anim->idleRunValue(idleWalkBlend);
		context.anim->sprintValue(sprintBlend);

		context.anim->rotateRootTowardsVector(
			context.input->getInputRelative3d(), delta, GETPARAM_D("animation", "rootRotationSpeed"));

		return {};
	}

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) {
		const auto delta = state->get_step();
		auto gravityDir = Vector3(0, -1, 0);
		auto& move = context.physics.movement;

		const auto pos = context.physics.position + Vector3(0, 1, 0);
		DebugDraw::Line(pos, pos + (move), Color(1, 1, 1), delta);

		for (int i = 0; i < state->get_contact_count(); i++) {
			auto pos = state->get_contact_collider_position(i);
			DebugDraw::Position(pos, Color(0, 1, 0), delta);

			auto normal = state->get_contact_local_normal(i);
			// Wall collision
			// if (g_up.dot(normal) < GETPARAM_F("floorMaxAngle")) {
			// 	// Adjust velocity along surface of collision if velocity is towards the wall.
			// 	const float walldot = normal.dot(move.normalized());

			// 	const auto rightOrtho = normal.cross(g_up).normalized();
			// 	const auto upOrtho = rightOrtho.cross(normal).normalized();

			// 	if (walldot < 0.f) {
			// 		const auto adjustment = move.project(normal);
			// 		move -= adjustment;
			// 	}
			// }
		}

		state->apply_central_force(gravityDir * (GETPARAMGLOBAL_D("gravityConstant") * GETPARAM_D("gravityScale")));
		move.y = state->get_linear_velocity().y;
		state->set_linear_velocity(move);

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		// direction
		utils::movementAcceleration(
			context, GETPARAM_D("onGroundAcceleration"), GETPARAM_D("onGroundDeceleration"), delta);

		// actions
		if (context.input->isActionPressed(EInputAction::JUMP)) {
			context.owner->apply_central_impulse(godot::Vector3(0, GETPARAM_D("jumpStrength"), 0));
			// return TInAirState();
		}
		if (context.input->isActionPressed(EInputAction::GRAPPLE) && context.grapple->getTarget()) {
			return TPreGrappleLaunchState();
		}
		if (context.input->isActionPressed(EInputAction::PARRY)) {
			return TParryPreState();
		}
		if (context.input->isActionPressed(EInputAction::ATTACK)) {
			return TAttackState();
		}
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
