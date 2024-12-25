#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <managers/inputManager.h>
#include <applicationtypes.hpp>
#include <configtypes.hpp>

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

	struct Data {
		int8_t coyoteframes;
		godot::Vector3 velocity;
	} data;

	const ConfigParam::Player& param = configparam.player;

public:
	TState getType() const override {
		return TOnGroundState();
	};

	TState enter(Context& context) override {
		context.anim->onGround();
		LOG(DEBUG, "state: ", Name())
		context.anim->onGround();
		// Immediate jump when entering while having just pressed jump
		if (context.input->isActionPressed(EInputAction::JUMP, 0.1f)) {
			context.physics.movement.y += param.jumpStrength();
			context.owner->set_linear_velocity(context.physics.movement);
			return TInAirState();
		}
		data.velocity = context.owner->get_linear_velocity();
		data.coyoteframes = param.coyoteframes();
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) {
		const auto delta = state->get_step();

		auto& move = context.physics.movement;
		const auto contactCount = state->get_contact_count();
		if (!contactCount) {
			data.coyoteframes = MAX(--data.coyoteframes, 0);
			if (!data.coyoteframes)
				return TInAirState();
		}
		for (int i = 0; i < contactCount; i++) {
			auto normal = state->get_contact_local_normal(i);
			// On floor
			if (g_up.dot(normal) > param.floorMaxAngle()) {
				data.velocity.y = 0;
				data.coyoteframes = param.coyoteframes();
			}
			else {
				auto pos = state->get_contact_local_position(i);
				if (pos.y < context.physics.position.y) {
					data.coyoteframes = MAX(--data.coyoteframes, 0);
					if (!data.coyoteframes)
						return TInAirState();
				}
			}
		}

		data.velocity.y -= context.physics.get.gravity() * delta;
		data.velocity.x = move.x;
		data.velocity.z = move.z;
		state->set_linear_velocity(data.velocity);

		// Disable friction when actively moving
		{
			auto mat = context.owner->get_physics_material_override();
			ASSERTNN(mat)
			if (data.velocity.length_squared() > 0.2f) {
				mat->set_friction(0.0);
			}
			else {
				mat->set_friction(1.0);
			}
		}

		_rotateRoot(context, delta);

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		// direction
		utils::movementAcceleration(context, param.onGroundAcceleration(), param.onGroundDeceleration(), delta);

		// actions
		if (context.input->isActionPressed(EInputAction::JUMP)) {
			data.velocity.y = param.jumpStrength();
			context.owner->set_linear_velocity(data.velocity);
			return TInAirState();
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

private:
	void _rotateRoot(Context& c, float delta) {
		const auto vel = c.owner->get_linear_velocity();
		const godot::Vector2 vel2d(vel.x, vel.z);
		const float speed = vel2d.length();

		float walkSpeed = param.walkSpeed();
		float sprintSpeed = configparam.player.sprintSpeed();
		float idleWalkBlend = godot::Math::clamp(speed / walkSpeed, 0.0f, 1.0f);
		float sprintBlend = godot::Math::clamp(
			(speed - walkSpeed) / (sprintSpeed - walkSpeed), 0.0f, 1.0f); // =0 for walkSpeed, 1 for sprintSpeed
		c.anim->idleRunValue(idleWalkBlend);
		c.anim->sprintValue(sprintBlend);

		c.anim->rotateRootTowardsVector(
			c.input->getInputRelative3d(), delta, GETPARAM_D("animation", "rootRotationSpeed"));
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
