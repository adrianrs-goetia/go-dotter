#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <debugdraw3d/api.h>
#include <managers/inputManager.h>
#include <configparams.hpp>

#include <components/animation.hpp>
#include <components/grappleInstigator.hpp>

namespace fsm::player {

class InAirState : public BaseState {
	TYPE(InAirState)

	struct Data {
		int jumpframes = 3;
	} data;

	ConfigParam::Player param;

public:
	TState getType() const override {
		return TInAirState();
	}

	bool canEnter() const override {
		return true;
	}

	TState enter(Context& context) override {
		std::holds_alternative<TParryJumpState>(context.states->get(1)) ? data.jumpframes = 0 : data.jumpframes = 3;
		context.anim->inAir();
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
			if (g_up.dot(normal) > param.floorMaxAngle()) {
				if (!data.jumpframes) {
					return TOnGroundState();
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

		// animation
		context.anim->rotateRootTowardsVector(
			context.input->getInputRelative3d(), delta, param.animation.rootRotationSpeed());

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		utils::movementAcceleration(context, param.inAirAcceleration(), param.inAirDeceleration(), delta);

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
