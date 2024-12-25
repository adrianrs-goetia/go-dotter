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

		auto impulse = utils::getInputOrForward(context);
		impulse *= param.impulse.xz();
		impulse.y = param.impulse.y();
		context.physics.movement = impulse;

		return {};
	}

	TState exit(Context& context) override {
		utils::enableCollision(context, m_clm);
		return {};
	}

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) override {
		const auto delta = state->get_step();
		context.physics.applyGravity(delta);
		state->set_linear_velocity(context.physics.movement);

		context.anim->rotateRootTowardsVector(getHorizontalUnit(state->get_linear_velocity()),
			delta,
			ConfigParam::Player::Animation::rootRotationSpeed());

		if (!m_enterTime.timestampWithinTimeframe(param.timeout())) {
			return TInAirState();
		}

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		utils::movementAcceleration(
			context, ConfigParam::Player::inAirAcceleration(), ConfigParam::Player::inAirDeceleration(), delta);

		return {};
	}
};

} //namespace fsm::player
