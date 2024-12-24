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

#include <godot_cpp/classes/physics_material.hpp>

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

	struct Get {
		float jumpStrenght() {
			return GETPARAM_F("jumpStrength");
		}

		auto coyoteFrames() {
			return GETPARAM_I("coyoteframes");
		}

		float walkSpeed() {
			return GETPARAM_F("walkSpeed");
		}

		float maxFloorAngle() {
			return GETPARAM_F("floorMaxAngle");
		}

		float onGroundAcceleration() {
			return GETPARAM_D("onGroundAcceleration");
		}

		float onGroundDeceleration() {
			return GETPARAM_D("onGroundDeceleration");
		}

		float animRootRotation() {
			return GETPARAM_D("animation", "rootRotationSpeed");
		}
	} get;

	void _rotateRoot(Context& c, float delta) {
		auto& vel = c.physics.velocity;
		const godot::Vector2 vel2d(vel.x, vel.z);
		const float speed = vel2d.length();
		float idleWalkBlend = godot::Math::clamp(speed / get.walkSpeed(), 0.0f, 1.0f);
		c.anim->idleRunValue(idleWalkBlend);
		c.anim->rotateRootTowardsVector(c.input->getInputRelative3d(), delta, get.animRootRotation());
	}

public:
	TState getType() const override {
		return TOnGroundState();
	};

	TState enter(Context& context) override {
		context.anim->onGround();
		LOG(DEBUG, "state: ", Name())
		// Immediate jump when entering while having just pressed jump
		if (context.input->isActionPressed(EInputAction::JUMP, 0.1f)) {
			context.physics.velocity.y += get.jumpStrenght();
			return TInAirState();
		}
		data.velocity = context.owner->get_linear_velocity();
		data.coyoteframes = get.coyoteFrames();
		return {};
	}

	TState exit(Context& context) override {
		context.physics.velocity = data.velocity;
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
			if (g_up.dot(normal) > get.maxFloorAngle()) {
				data.velocity.y = 0;
				data.coyoteframes = get.coyoteFrames();
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
		utils::movementAcceleration(context, get.onGroundAcceleration(), get.onGroundDeceleration(), delta);

		// actions
		if (context.input->isActionPressed(EInputAction::JUMP)) {
			data.velocity.y = get.jumpStrenght();
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
};

} //namespace fsm::player

#undef CONFIG_PREFIX
