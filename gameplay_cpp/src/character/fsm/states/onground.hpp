#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <managers/inputManager.h>
#include <configparams.hpp>

#include <applicationparams.hpp>
#include <configparams.hpp>

#include <components/animation.hpp>
#include <components/grappleInstigator.hpp>
#include <components/parryInstigator.hpp>
#include <components/parryTarget.hpp>

namespace fsm::player {

class OnGroundState : public BaseState {
	TYPE(OnGroundState)

	struct Data {
		int8_t coyoteframes;
	} data;

	ConfigParam::Player param;

public:
	TState getType() const override {
		return TOnGroundState();
	};

	TState enter(Context& context) override {
		context.anim->onGround();
		// Immediate jump when entering while having just pressed jump
		if (context.input->isActionPressed(EInputAction::JUMP, 0.1f)) {
			context.physics.movement.y += param.jumpStrength();
			context.owner->set_linear_velocity(context.physics.movement);
			return TInAirState();
		}
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
			// if (utils::projectileCollision(state->get_contact_collider_object(i))) {
			// Check type of object collision
			if (auto* projectile = godot::Object::cast_to<Projectile>(state->get_contact_collider_object(i))) {
				_projectileCollision(context, *projectile);
			}

			// On floor
			if (g_up.dot(normal) > param.floorMaxAngle()) {
				move.y = 0;
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

		context.physics.applyGravity(delta);
		state->set_linear_velocity(move);

		// Disable friction when actively moving
		{
			auto mat = context.owner->get_physics_material_override();
			ASSERTNN(mat)
			if (move.length_squared() > 0.2f) {
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
			context.physics.movement.y = param.jumpStrength();
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
		float sprintSpeed = param.sprintSpeed();
		float idleWalkBlend = godot::Math::clamp(speed / walkSpeed, 0.0f, 1.0f);
		float sprintBlend = godot::Math::clamp(
			(speed - walkSpeed) / (sprintSpeed - walkSpeed), 0.0f, 1.0f); // =0 for walkSpeed, 1 for sprintSpeed
		c.anim->idleRunValue(idleWalkBlend);
		c.anim->sprintValue(sprintBlend);

		c.anim->rotateRootTowardsVector(c.input->getInputRelative3d(), delta, param.animation.rootRotationSpeed());
	}

	void _projectileCollision(Context& c, Projectile& projectile) {
		projectile.handleExternalEvent(EventPlayerCollision{});
		const bool isdead = c.gui->decrement();
		if (isdead) {
			LOG(WARN, "Player has OFFICIALLY died")
			return;
		}
	}
};

} //namespace fsm::player
