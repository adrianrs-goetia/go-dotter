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

	struct Data {
		int jumpframes = 3;
		godot::Vector3 velocity;
	} data;

	struct Get {
		float walkSpeed() {
			return GETPARAM_F("walkSpeed");
		}

		float maxFloorAngle() {
			return GETPARAM_F("floorMaxAngle");
		}

		float inAirAcceleration() {
			return GETPARAM_D("inAirAcceleration");
		}

		float inAirDeceleration() {
			return GETPARAM_D("inAirDeceleration");
		}

		float animRootRotation() {
			return GETPARAM_D("animation", "rootRotationSpeed");
		}
	} get;

public:
	TState getType() const override {
		return TInAirState();
	}

	bool canEnter() const override {
		return true;
	}

	TState enter(Context& context) override {
		LOG(DEBUG, "state: ", Name())
		data.jumpframes = 3;
		data.velocity = context.owner->get_linear_velocity();
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) override {
		const auto delta = state->get_step();
		auto& move = context.physics.movement;
		data.jumpframes = MAX(--data.jumpframes, 0);

		for (int i = 0; i < state->get_contact_count(); i++) {
			auto normal = state->get_contact_local_normal(i);
			// Land on floor
			if (g_up.dot(normal) > get.maxFloorAngle()) {
				if (!data.jumpframes) {
					return TOnGroundState();
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

		// animation
		context.anim->rotateRootTowardsVector(
			context.input->getInputRelative3d(), delta, GETPARAM_D("animation", "rootRotationSpeed"));

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		utils::movementAcceleration(context, get.inAirAcceleration(), get.inAirDeceleration(), delta);

		// if (context.input->isActionPressed(EInputAction::GRAPPLE) && context.grapple->getTarget()) {
		// 	return TPreGrappleLaunchState();
		// }
		if (context.input->isActionPressed(EInputAction::PARRY)) {
			return TParryPreState();
		}
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
