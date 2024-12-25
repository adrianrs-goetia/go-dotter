#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <managers/inputManager.h>
#include <configparams.hpp>

#include <components/animation.hpp>
#include <components/parryInstigator.hpp>
#include <events/parry.hpp>

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>

namespace fsm::player {

class ParryJumpState : public BaseState {
	TYPE(ParryJumpState)

	ConfigParam::Player::Parry::Jump param;

private:
	Timestamp m_enterTime;
	Timestamp m_intangibilityTime;
	utils::CollisionLayerMask m_clm;
	struct {
		godot::Vector3 velocity;
	} data;

public:
	TState getType() const override {
		return TParryJumpState();
	}

	bool canEnter() const override {
		return true;
	}

	TState enter(Context& context) override {
		context.anim->doParry(true, false); //only animate upper
		m_enterTime.setTimestamp();

		m_clm = utils::disableCollision(context);

		auto* target = context.parry->getLastParryContactAssert();
		const auto targetPos = target->get_global_position() + godot::Vector3(0, 1, 0); // add margin to go above target
		const auto length = godot::Vector3(targetPos - context.physics.position);
		const auto dir = length.normalized();

		data.velocity = dir * length.length() * param.impulse();
		return {};
	}

	TState exit(Context& context) override {
		utils::enableCollision(context, m_clm);
		return {};
	}

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) override {
		const auto delta = state->get_step();
		data.velocity.y -= context.physics.get.gravity() * delta;
		state->set_linear_velocity(data.velocity);

		context.anim->rotateRootTowardsVector(getHorizontalUnit(state->get_linear_velocity()),
			delta,
			ConfigParam::Player::Animation::rootRotationSpeed());

		if (!m_enterTime.timestampWithinTimeframe(param.timeout())) {
			return TInAirState();
		}

		if (!m_intangibilityTime.timestampWithinTimeframe(param.intagibilityTime())) {
			utils::enableCollision(context, m_clm);
		}

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		// @todo: should auto leave state after a certain point along the jump curve?

		if (context.input->isActionPressed(EInputAction::JUMP)) {
			// @todo: check if target is within range
			auto target = context.parry->getLastParryContactAssert();
			target->onAction({ EventParryJump() });

			auto newVel = context.input->getInputRelative3d(); // Expected to be horizontal
			newVel *= param.doubleJumpHorizontalStrength();
			newVel.y = param.doubleJumpImpulse();
			context.owner->set_linear_velocity(newVel);
			return TInAirState();
		}

		return {};
	}
};

} //namespace fsm::player
